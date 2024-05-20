#include <gstreamer-1.0/gst/gst.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <linux/videodev2.h>
#include <gstreamer-1.0/gst/video/video.h>
#include "v4l2-extra.h"
#include <sys/ioctl.h>
#include <dirent.h>
#include <time.h>

static int camera_fd = -1;

static void process_command(const char *cmd)
{
	if (0 == strcasecmp("af",cmd)) {
		struct v4l2_control c;
		c.id=V4L2_CID_AUTO_FOCUS_START;
		int ret=ioctl(camera_fd,VIDIOC_S_CTRL,(v4l2_control *)&c);
		if(ret==-1)
			printf("\n ioctl V4L2_CID_AUTO_FOCUS_START fail: %m\n ");
		else
			printf("\nioctl V4L2_CID_AUTO_FOCUS_START sucessful\n");
	} else if (0 == strncasecmp("sp",cmd,2)) {
		struct v4l2_send_command_control vc;
		vc.id = 105;
		vc.value0 = strtoul(cmd+2,0,0);
		int ret = ioctl(camera_fd,VIDIOC_SEND_COMMAND,(v4l2_send_command_control*)&vc);
		if(ret==-1)
			printf("\n ioctl STEP fail: %m\n ");
		else
			printf("\nioctl STEP sucessful: %d\n",ret);
	} else if (0 == strncasecmp("fd",cmd,2)) {
		camera_fd = strtoul(cmd+2,0,0);
		printf("camera_fd set to %d\n", camera_fd);
	} else if (0 == strncasecmp("wt",cmd,2)) {
		sleep(strtoul(cmd+2,0,0));
	} else
                printf("unknown command %s\n", cmd);
}

int main (int argc, char const *const argv[])
{
	printf("Hello %s\n", argv[0]);
	gst_init(NULL, NULL);
	
	const char *dev = getenv("CAM_DEVICE");
	if (NULL == dev) {
		printf("CAM_DEVICE not found in the environment, using default.\n");
		dev = "/dev/video0";
	}
	printf("Video device: %s\n", dev);

	char pipeline_string[1024];
    snprintf(pipeline_string, sizeof(pipeline_string), "v4l2src device=%s ! videoconvert ! autovideosink sync=false ", dev);


	printf("Pipeline string is: %s\n", pipeline_string);

	// Create pipeline from configuration string
	GstElement *pipeline = gst_parse_launch(pipeline_string, NULL);
	printf( "pipeline == %p\n", pipeline);
	GstElement *sink = gst_bin_get_by_name(GST_BIN(pipeline), "sink");

	// Retrieve pipeline signal bus
	GstBus *bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline));
	printf( "bus == %p\n", bus);

	// Set pipeline to ready
	gst_element_set_state(pipeline, GST_STATE_READY);

	printf("pipeline is ready\n");
	gst_element_set_state(pipeline, GST_STATE_PLAYING);

	char inbuf[80];
	char output[80];
	for (int arg=1; arg < argc ; arg++) {
		process_command(argv[arg]);
	}
	return 0 ;
}
