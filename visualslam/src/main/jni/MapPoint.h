// This file declares the MapPoint class
// 
// The map is made up of a bunch of mappoints.
// Each one is just a 3D point in the world;
// it also includes information on where and in which key-frame the point was
// originally made from, so that pixels from that keyframe can be used
// to search for that point.
// Also stores stuff like inlier/outlier counts, and privat information for 
// both Tracker and MapMaker.

#ifndef __MAP_POINT_H
#define __MAP_POINT_H
#define EIGEN_DONT_ALIGN_STATICALLY True

#include <set>
#include "eigen3/Eigen/Dense"



class KeyFrame;
class TrackerData;
class MapMakerData;

struct MapPoint {
  // Constructor inserts sensible defaults and zeros pointers.
  inline MapPoint() {
    bBad = false;
    pTData = NULL;
    pMMData = NULL;
    nMEstimatorOutlierCount = 0;
    nMEstimatorInlierCount = 0;
  }
  
  // Where in the world is this point? The main bit of information, really.
  Eigen::Vector3d v3WorldPos;
  // Is it a dud? In that case it'll be moved to the trash soon.
  bool bBad;
  
  // What pixels should be used to search for this point?
  KeyFrame *pPatchSourceKF; // The KeyFrame the point was originally made in
  int nSourceLevel;         // Pyramid level in source KeyFrame
  Eigen::Vector2d irCenter;   // This is in level-coords in the source pyramid level
  int rcolor;
  int gcolor;
  int bcolor;

  // What follows next is a bunch of intermediate vectors - they all lead up
  // to being able to calculate v3Pixel{Down,Right}_W, which the PatchFinder
  // needs for patch warping!
  Eigen::Vector3d v3Center_NC;             // Unit vector in Source-KF coords pointing at the patch center
  Eigen::Vector3d v3OneDownFromCenter_NC;  // Unit vector in Source-KF coords pointing towards one pixel down of the patch center
  Eigen::Vector3d v3OneRightFromCenter_NC; // Unit vector in Source-KF coords pointing towards one pixel right of the patch center
  Eigen::Vector3d v3Normal_NC;

  Eigen::Vector3d v3PixelDown_W;           // 3-Vector in World coords corresponding to a one-pixel move down the source image
  Eigen::Vector3d v3PixelRight_W;          // 3-Vector in World coords corresponding to a one-pixel move right the source image
  void RefreshPixelVectors();        // Calculates above two vectors
  
  // Info for the Mapmaker (not to be trashed by the tracker:)
  MapMakerData *pMMData;
  
  // Info for the Tracker (not to be trashed by the MapMaker:)
  TrackerData *pTData;
  
  // Info provided by the tracker for the mapmaker:
  int nMEstimatorOutlierCount;
  int nMEstimatorInlierCount;
  
  // Random junk (e.g. for visualisation)
  double dCreationTime;
};

#endif
