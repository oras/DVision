v4l2-ctl -d /dev/video1 -i 1
v4l2-ctl --set-fmt-video=width=640,height=480
v4l2-ctl --set-parm=25
v4l2-ctl -d /dev/video1 --set-standard=1



