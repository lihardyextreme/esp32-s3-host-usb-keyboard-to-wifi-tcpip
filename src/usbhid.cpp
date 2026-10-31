#include <Arduino.h>

#include "usb/usb_host.h"

#include "hid_host.h"
#include "hid_usage_keyboard.h"
#include "hid_usage_mouse.h"


void blink();
size_t sendAsyncData(const uint8_t *const data, const int length);
size_t sendAsyncChar(char);

typedef struct {
  hid_host_device_handle_t hid_device_handle;
  hid_host_driver_event_t event;
  void *arg;
} hid_host_event_queue_t;

QueueHandle_t hid_host_event_queue;

void hid_host_keyboard_report_callback(const uint8_t *const data, const int length) {
  Serial.print("keyboard[");
  Serial.print(length);
  Serial.print("]: ");
  for(int i=0;i<length;i++) {
    Serial.print(data[i]);
    Serial.print(",");
  }
  Serial.println(".");
  if(sizeof(hid_keyboard_input_report_boot_t)!=length) {
    Serial.printf("ERROR: size does not match %d != %d \r\n", sizeof(hid_keyboard_input_report_boot_t), length);
    return;
  }
  sendAsyncData(data,length);
  blink();
}

void hid_host_mouse_report_callback(const uint8_t *const data, const int length) {
  Serial.print("mouse[");
  Serial.print(length);
  Serial.print("]: ");
  for(int i=0;i<length;i++) {
    Serial.print(data[i]);
    Serial.print(",");
  }
  Serial.println(".");
  blink();
}

void hid_host_generic_report_callback(const uint8_t *const data, const int length) {
  Serial.println("hid_host_generic_report_callback ????");
}

void hid_host_interface_callback(hid_host_device_handle_t hid_device_handle,
                                 const hid_host_interface_event_t event,
                                 void *arg) {
  uint8_t data[64] = {0};
  size_t data_length = 0;
  hid_host_dev_params_t dev_params;
  ESP_ERROR_CHECK(hid_host_device_get_params(hid_device_handle, &dev_params));

  switch (event) {
  case HID_HOST_INTERFACE_EVENT_INPUT_REPORT:
    ESP_ERROR_CHECK(hid_host_device_get_raw_input_report_data(
        hid_device_handle, data, 64, &data_length));

    if (HID_SUBCLASS_BOOT_INTERFACE == dev_params.sub_class) {
      if (HID_PROTOCOL_KEYBOARD == dev_params.proto) {
        hid_host_keyboard_report_callback(data, data_length);
      } else if (HID_PROTOCOL_MOUSE == dev_params.proto) {
        hid_host_mouse_report_callback(data, data_length);
      }
    } else {
      hid_host_generic_report_callback(data, data_length);
    }

    break;
  case HID_HOST_INTERFACE_EVENT_DISCONNECTED:
    Serial.println("disconnected");
    //ESP_LOGI(TAG, "HID Device, protocol '%s' DISCONNECTED",
    //         hid_proto_name_str[dev_params.proto]);
    ESP_ERROR_CHECK(hid_host_device_close(hid_device_handle));
    break;
  case HID_HOST_INTERFACE_EVENT_TRANSFER_ERROR:
    Serial.println("tranf. err");
    //ESP_LOGI(TAG, "HID Device, protocol '%s' TRANSFER_ERROR",
    //         hid_proto_name_str[dev_params.proto]);
    break;
  default:
    Serial.println("unknown event");
    //ESP_LOGE(TAG, "HID Device, protocol '%s' Unhandled event",
    //         hid_proto_name_str[dev_params.proto]);
    break;
  }

}

void hid_host_device_event(hid_host_device_handle_t hid_device_handle,
                           const hid_host_driver_event_t event, void *arg) {
  hid_host_dev_params_t dev_params;
  ESP_ERROR_CHECK(hid_host_device_get_params(hid_device_handle, &dev_params));
  const hid_host_device_config_t dev_config = {
      .callback = hid_host_interface_callback, .callback_arg = NULL};

  Serial.print("hid_host_device_event: ");
  Serial.println(event);
  switch (event) {
  case HID_HOST_DRIVER_EVENT_CONNECTED:
    Serial.print("connected: ");
    Serial.println(dev_params.proto);

    ESP_ERROR_CHECK(hid_host_device_open(hid_device_handle, &dev_config));
    Serial.println("hid_host_device_open");
    if (HID_SUBCLASS_BOOT_INTERFACE == dev_params.sub_class) {
        Serial.println("->HID_SUBCLASS_BOOT_INTERFACE");
      ESP_ERROR_CHECK(hid_class_request_set_protocol(hid_device_handle,
                                                     HID_REPORT_PROTOCOL_BOOT));
      Serial.println("hid_class_request_set_protocol");
      if (HID_PROTOCOL_KEYBOARD == dev_params.proto) {
        Serial.println("-->HID_PROTOCOL_KEYBOARD");
        ESP_ERROR_CHECK(hid_class_request_set_idle(hid_device_handle, 0, 0));
        Serial.println("hid_class_request_set_idle");
      }
      if (HID_PROTOCOL_MOUSE == dev_params.proto) {
        Serial.println("-->HID_PROTOCOL_KEYBOARD - TODO");
      }
    }
    ESP_ERROR_CHECK(hid_host_device_start(hid_device_handle));
    Serial.println("hid_host_device_start!");
    break;
  default:
    break;
  }
}

static void usb_lib_task(void *arg) {

  const usb_host_config_t host_config = {
      .skip_phy_setup = false,
      .intr_flags = ESP_INTR_FLAG_LEVEL1,
  };

  ESP_ERROR_CHECK(usb_host_install(&host_config));
  xTaskNotifyGive((TaskHandle_t)arg);
  Serial.println("usb_host_install");

  while (true) {
    uint32_t event_flags;
    usb_host_lib_handle_events(portMAX_DELAY, &event_flags);

    if (event_flags & USB_HOST_LIB_EVENT_FLAGS_NO_CLIENTS) {
      usb_host_device_free_all();
      Serial.println("USB Event flags: NO_CLIENTS");
    }
    if (event_flags & USB_HOST_LIB_EVENT_FLAGS_ALL_FREE) {
      Serial.println("USB Event flags: ALL_FREE");
    }
  }
}

void hid_host_task(void *pvParameters) {
  hid_host_event_queue_t evt_queue;
  hid_host_event_queue = xQueueCreate(10, sizeof(hid_host_event_queue_t));

  while (true) {
    if (xQueueReceive(hid_host_event_queue, &evt_queue, pdMS_TO_TICKS(50))) {
      hid_host_device_event(evt_queue.hid_device_handle, evt_queue.event, evt_queue.arg);
    }
  }
}

void hid_host_device_callback(hid_host_device_handle_t hid_device_handle,
                              const hid_host_driver_event_t event, void *arg) {
  const hid_host_event_queue_t evt_queue = {
      .hid_device_handle = hid_device_handle, .event = event, .arg = arg};
  xQueueSend(hid_host_event_queue, &evt_queue, 0);
}

void setupUsbHid()
{
      BaseType_t task_created;

  task_created =
      xTaskCreatePinnedToCore(usb_lib_task, "usb_events", 4096,
                              xTaskGetCurrentTaskHandle(), 2, NULL, 0);
  assert(task_created == pdTRUE);

  Serial.println("usb_lib_task");

  ulTaskNotifyTake(false, 1000);

  const hid_host_driver_config_t hid_host_driver_config = {
      .create_background_task = true,
      .task_priority = 5,
      .stack_size = 4096,
      .core_id = 0,
      .callback = hid_host_device_callback,
      .callback_arg = NULL};

  ESP_ERROR_CHECK(hid_host_install(&hid_host_driver_config));

  Serial.println("hid_host_install");

  task_created =
      xTaskCreate(&hid_host_task, "hid_task", 4 * 1024, NULL, 2, NULL);
  assert(task_created == pdTRUE);

  Serial.println("hid_host_task");

}