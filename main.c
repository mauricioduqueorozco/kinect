#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libusb-1.0/libusb.h>

#include <pthread.h>

#ifdef __APPLE__
  // #include <GLUT/glut.h>
  // #include <OpenGL/gl.h>
  // #include <OpenGL/glu.h>
  // #include </usr/local/Cellar/libusb/1.0.22/include/libusb-1.0/libusb.h>
#else
  // #include <GL/glut.h>
  // #include <GL/gl.h>
  // #include <GL/glu.h>
#endif

void print_devices(libusb_device *dev){
  struct libusb_device_descriptor desc;
  struct libusb_config_descriptor *config;
  const struct libusb_interface *inter;
  const struct libusb_interface_descriptor *interdesc;
  const struct libusb_endpoint_descriptor *endpointdesc;

  int ret;
  int i,j,k;
  ret = libusb_get_device_descriptor(dev, &desc);
  if(ret < 0){
    fprintf(stderr, "%s\n", "Error");
    return;
  }
  printf("Number of pos configs is %d\n", desc.bNumConfigurations);
  printf("Device class: %hu\n", desc.idVendor);
  printf("Product ID: %hu\n", desc.idProduct);

  libusb_get_config_descriptor(dev, 0, &config);

  printf("Interfaces: %hhu\n", config->bNumInterfaces);

  for (i = 0; i < config->bNumInterfaces; i++) {
    inter = &config->interface[i];
    printf("number of alt settings: %d\n", inter->num_altsetting);
    for (j = 0; j < inter->num_altsetting; j++) {
      interdesc = &inter->altsetting[j];
      printf("interface number: %hhu\n", interdesc->bInterfaceNumber);
      printf("Num of endpoints: %hhu\n", interdesc->bNumEndpoints);
      for (size_t k = 0; k < interdesc->bNumEndpoints; k++) {
        endpointdesc = &interdesc->endpoint[k];
        printf("   Desc Type: %hhu\n", endpointdesc->bDescriptorType);
        printf("   EP Addr: %hhu\n", endpointdesc->bEndpointAddress);
      }
    }
  }
  printf("\n\n");
  libusb_free_config_descriptor(config);
}

int main(int argc, char **argv){
    printf("%s\n", "Starting program");
    libusb_device **devices;
    libusb_context *context = NULL;

    ssize_t list = 0;
    size_t i;
    int ret;

    ret = libusb_init(&context);
    if(ret < 0){
      perror("libusb_init");
      exit(1);
    }

    list = libusb_get_device_list(context, &devices);
    if(list < 0){
      fprintf(stderr, "%s\n", "Error in getting device list");
      libusb_free_device_list(devices, 1);
      libusb_exit(context);
      exit(1);
    }
    printf("%zd\n", list);

    for (int i = 0; i < list; i++) {
      print_devices(devices[i]);
    }
    libusb_free_device_list(devices, 1);
    libusb_exit(context);
    return 0;
}
