# new_scan
Replace inf with TRAJECTORY_BUILDER_2D.max_range - 0.01 in scan data, 
so that the area in which lidar has no reflection can be cleared out, 
otherwise it will remain unknown.

Launch
---

```
	roslaunch new_scan new_scan.launch 
```
