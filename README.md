# PjWebrtc

## 功能
M90版本的webrtc编译库。

## 编译方法
```
gn gen --target=x64 --ide=vs2019 --args="is_debug=true rtc_enable_protobuf=false is_clang=false target_cpu=\"x64\"  enable_iterator_debugging=true  use_custom_libcxx=false symbol_level=0 rtc_include_tests=false" out/debug_x64
ninja -C out/debug_x64

增加h264和忽略错误treat_warnings_as_errors
gn gen --target=x64 --ide=vs2019 --args="is_debug=true rtc_enable_protobuf=false is_clang=false target_cpu=\"x64\"  enable_iterator_debugging=true  use_custom_libcxx=false symbol_level=0 rtc_include_tests=false use_openh264=true treat_warnings_as_errors = false" out/deb
ug_onh264_x64
```

## 调用方法
将peerconnectionclient单独摘出来，调用webrtc.lib
