#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libusb-1.0/libusb.h>

#include <pthread.h>

#ifdef __APPLE__
  #include <GLUT/glut.h>
  #include <OpenGL/gl.h>
  #include <OpenGL/glu.h>
  // #include <OpenGL/gl3.h>
  // #define __gl_h_
  // #include <GLUT/glut.h>
  // #include </usr/local/Cellar/libusb/1.0.22/include/libusb-1.0/libusb.h>
#else
  // #include <GL/glut.h>
  // #include <GL/gl.h>
  // #include <GL/glu.h>
#endif

pthread_t gl_thread;

int g_argc;
char **g_argv;

int window;

void InitGL(int Width, int Height)
{
	// glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	// glClearDepth(1.0);
	// glDepthFunc(GL_LESS);
	// glDisable(GL_DEPTH_TEST);
	// glEnable(GL_BLEND);
	// glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// glShadeModel(GL_SMOOTH);
	// glGenTextures(1, &gl_depth_tex);
	// glBindTexture(GL_TEXTURE_2D, gl_depth_tex);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// glGenTextures(1, &gl_rgb_tex);
	// glBindTexture(GL_TEXTURE_2D, gl_rgb_tex);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// ReSizeGLScene(Width, Height);
}


void *gl_threadfunc(void *arg){
  printf("%s\n\n", "GL thread");
  glutInit(&g_argc, g_argv);

  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
  glutInitWindowSize(1280, 480);
  glutInitWindowPosition(0, 0);

  window = glutCreateWindow("SolidKnect");
  // glutDisplayFunc(&DrawGLScene);
  // glutIdleFunc(&DrawGLScene);
  // glutReshapeFunc(&ReSizeGLScene);
  // glutKeyboardFunc(&keyPressed);
  InitGL(1280, 480);

  return NULL;
}

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
    printf("%s\n", "Starting program\n\n");
    libusb_device **devices;
    libusb_context *context = NULL;
    libusb_device_handle *dev;

    ssize_t list = 0;
    size_t i;
    int ret;
    int res;

    g_argc = argc;
    g_argv = argv;

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
    printf("List of devices connected: %zd\n", list);

    dev = libusb_open_device_with_vid_pid(NULL, 0x45e, 0x2ae);
    if(!dev){
      perror("Cannot connect to Kinect device, make sure it is connected\n\n");
      exit(1);
    }else{
      printf("Connection success with Kinect device\n\n");
    }

    res = pthread_create(&gl_thread, NULL, gl_threadfunc, NULL);
    if (res) {
      printf("pthread_create failed\n");
      return 1;
    }

    for (int i = 0; i < list; i++) {
      print_devices(devices[i]);
    }
    libusb_free_device_list(devices, 1);
    libusb_exit(context);
    return 0;
}
