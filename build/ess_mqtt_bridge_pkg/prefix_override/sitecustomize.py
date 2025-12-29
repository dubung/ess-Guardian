import sys
if sys.prefix == '/usr':
    sys.real_prefix = sys.prefix
    sys.prefix = sys.exec_prefix = '/home/ros/2512_4th_proj_chanmi/install/ess_mqtt_bridge_pkg'
