#include <libfreenect.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

const uint32_t RGB = 0x00;
const uint32_t DEPTH = 0x01;
char frame_buf[7];

void rgb_cb (freenect_device *dev, void *video, uint32_t timestamp) {
  size_t len = 640*480*3;
  freenect_set_video_buffer(dev, video);
  // varint-encoded 640*480*3+4:
  frame_buf[0] = 132;
  frame_buf[1] = 160;
  frame_buf[2] = 56;
  ((uint32_t*)(frame_buf+3))[0] = RGB;
  fwrite(frame_buf, sizeof(char), 7, stdout);
  for (uint32_t i = 0; i < 640*480*3; i += 4096) {
    fwrite((void*)(((char*)video)+i), sizeof(char), 4096, stdout);
  }
}

void depth_cb (freenect_device *dev, void *v_depth, uint32_t timestamp) {
}

int main (int argc, char **argv) {
  freenect_context *f_ctx;
  freenect_device *f_dev;
  if (freenect_init(&f_ctx, NULL) < 0) {
    printf("failed to initialize context\n");
    return 1;
  }
  if (freenect_open_device(f_ctx, &f_dev, 0) < 0) {
    printf("could not open the device\n");
    freenect_shutdown(f_ctx);
    return 1;
  }

  freenect_set_tilt_degs(f_dev,0);
  freenect_set_led(f_dev,LED_RED);
  freenect_set_depth_callback(f_dev, depth_cb);
  freenect_set_depth_mode(f_dev, freenect_find_depth_mode(FREENECT_RESOLUTION_MEDIUM, FREENECT_DEPTH_11BIT_PACKED));
  freenect_set_video_callback(f_dev,rgb_cb);
  freenect_start_depth(f_dev);
  freenect_start_video(f_dev);

  while (freenect_process_events(f_ctx) >= 0);
  return 0;
}
