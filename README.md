# new_scan
Replace inf with TRAJECTORY_BUILDER_2D.max_range - 0.01 in scan data, 
so that the area in which lidar has no reflection can be cleared out, 
otherwise it will remain unknown.

Configuration
---
In new_scan/launch/new_scan.launch:

"robot_scan_topic" is the name of your scan topic removing its prefix,
"robot_namespace"  is the identical part of your robot namespace,
"new_scan_topic"   is the new scan topic name.
For example, assume that I have tb3_0/scan, tb3_1/scan, tb3_2/scan now,
then the correct configuration is as follows:
```
	<param name="robot_scan_topic"  value="scan"/>
	<param name="robot_namespace"   value="tb3"/>
	<param name="new_scan_topic"    value="new_scan"/>
```
And tb3_0/new_scan, tb3_1/new_scan, tb3_2/new_scan will be generated.


Launch
---

```
	roslaunch new_scan new_scan.launch 
```

Details
---
The out-of-range scan data such as inf will be discarded, and any scan data 
between max_range and missing_data_ray_length will be used to clear out 
probability-unknown grids.
