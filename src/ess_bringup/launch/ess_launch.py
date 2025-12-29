from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        Node(
            package='ess_hardware_pkg',
            executable='photo_sensor_node',
            output='screen',
        ),
        Node(
            package='ess_hardware_pkg',
            executable='lifter_node',
            output='screen',
        ),
       # Node(
       #     package='ess_control_pkg',
       #     executable='lifter_node',
       #     output='screen',
       # ),
    ])

