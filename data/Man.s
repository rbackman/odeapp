# SK Skeleton Definition - M. Kallmann 2004
# File generated with export_sk_v.mel

set_name Man

globalgeo true
skeleton
root "Hips"
{ offset 0 0 0
  channel XPos 0 free
  channel YPos 0 free
  channel ZPos 0 free
  euler XYZ
   channel XRot 0  free
   channel YRot 0  free
   channel ZRot 0  free
 
  joint "RightUpLeg"
  { offset -0.4 0.30321 0.03515
    channel XPos 0
    channel YPos 0
    channel ZPos 0
    euler XYZ
    channel XRot 0  free
    channel YRot 0  free
    channel ZRot 0  free

    joint "RightLeg"
    { offset 0 -1.85938 0
      channel XPos 0
      channel YPos 0
      channel ZPos 0
      euler XYZ
      channel XRot 0  free
      channel YRot 0  free
      channel ZRot 0  free

      joint "RightFoot"
      { offset 0 -2.24432 0
        channel XPos 0
        channel YPos 0
        channel ZPos 0
        euler XYZ
        channel XRot 0  free
        channel YRot 0  free
        channel ZRot 0  free

        visgeo "visGeo_RightFootMesh.obj"
        joint "RightToe"
        { offset 0 1e-005 0.70141
          channel XPos 0
          channel YPos 0
          channel ZPos 0
          euler XYZ
          channel XRot 0  free
          channel YRot 0  free
          channel ZRot 0  free

            joint "RightToeTip"
            { offset -0.00445 9e-005 0.46875
              channel XPos 0
              channel YPos 0
              channel ZPos 0
              euler XYZ
              channel XRot 0  free
              channel YRot 0  free
              channel ZRot 0  free

            }
        }
        joint "RightHeelTip"
        { offset 0 1e-005 -0.39716
          channel XPos 0
          channel YPos 0
          channel ZPos 0
          euler XYZ
          channel XRot 0  free
          channel YRot 0  free
          channel ZRot 0  free

        }
      }
    }
  }
  joint "LeftUpLeg"
  { offset 0.4 0.30321 0.03515
    channel XPos 0
    channel YPos 0
    channel ZPos 0
    euler XYZ
    channel XRot 0  free
    channel YRot 0  free
    channel ZRot 0  free

    joint "LeftLeg"
    { offset 0 -1.85938 0
      channel XPos 0
      channel YPos 0
      channel ZPos 0
      euler XYZ
      channel XRot 0  free
      channel YRot 0  free
      channel ZRot 0  free

      joint "LeftFoot"
      { offset 0 -2.24431 0
        channel XPos 0
        channel YPos 0
        channel ZPos 0
        euler XYZ
        channel XRot 0  free
        channel YRot 0  free
        channel ZRot 0  free

        visgeo "visGeo_LeftFootMesh.obj"
        joint "LeftToe"
        { offset 0 0 0.70141
          channel XPos 0
          channel YPos 0
          channel ZPos 0
          euler XYZ
          channel XRot 0  free
          channel YRot 0  free
          channel ZRot 0  free

            joint "LeftToeTip"
            { offset 0 0 0.46875
              channel XPos 0
              channel YPos 0
              channel ZPos 0
              euler XYZ
              channel XRot 0  free
              channel YRot 0  free
              channel ZRot 0  free

            }
        }
        joint "LeftHeelTip"
        { offset 0 0 -0.39716
          channel XPos 0
          channel YPos 0
          channel ZPos 0
          euler XYZ
          channel XRot 0  free
          channel YRot 0  free
          channel ZRot 0  free

        }
      }
    }
  }
  joint "Spine1"
  { offset 0 0.10639 0
    channel XPos 0
    channel YPos 0
    channel ZPos 0
    euler XYZ
    channel XRot 0  free
    channel YRot 0  free
    channel ZRot 0  free

    joint "Spine2"
    { offset 0 1.00023 0
      channel XPos 0
      channel YPos 0
      channel ZPos 0
      euler XYZ
      channel XRot 0  free
      channel YRot 0  free
      channel ZRot 0  free

      joint "Shoulder"
      { offset 0 0.79126 0
        channel XPos 0
        channel YPos 0
        channel ZPos 0
        euler XYZ
        channel XRot 0  free
        channel YRot 0  free
        channel ZRot 0  free

        joint "Head"
        { offset 0 0.72357 0
          channel XPos 0
          channel YPos 0
          channel ZPos 0
          euler XYZ
          channel XRot 0  free
          channel YRot 0  free
          channel ZRot 0  free

            joint "HeadTop"
            { offset 0 1.39387 0
              channel XPos 0
              channel YPos 0
              channel ZPos 0
              euler XYZ
              channel XRot 0  free
              channel YRot 0  free
              channel ZRot 0  free

            }
        }
        joint "RightUpArm"
        { offset -1.06075 0.38919 0
          channel XPos 0
          channel YPos 0
          channel ZPos 0
          euler XYZ
          channel XRot 0  free
          channel YRot 0  free
          channel ZRot 0  free

          joint "RightArm"
          { offset 0 -1.59113 0
            channel XPos 0
            channel YPos 0
            channel ZPos 0
            euler XYZ
            channel XRot 0  free
            channel YRot 0  free
            channel ZRot 0  free

            joint "RightHand"
            { offset 0 -1.47544 0
              channel XPos 0
              channel YPos 0
              channel ZPos 0
              euler XYZ
              channel XRot 0  free
              channel YRot 0  free
              channel ZRot 0  free

              visgeo "visGeo_RightHandMesh.obj"
            }
          }
        }
        joint "LeftUpArm"
        { offset 1.06075 0.38919 0
          channel XPos 0
          channel YPos 0
          channel ZPos 0
          euler XYZ
          channel XRot 0  free
          channel YRot 0  free
          channel ZRot 0  free

          joint "LeftArm"
          { offset 0 -1.59113 0
            channel XPos 0
            channel YPos 0
            channel ZPos 0
            euler XYZ
            channel XRot 0  free
            channel YRot 0  free
            channel ZRot 0  free

            joint "LeftHand"
            { offset -0.00391 -1.47542 0
              channel XPos 0
              channel YPos 0
              channel ZPos 0
              euler XYZ
              channel XRot 0  free
              channel YRot 0  free
              channel ZRot 0  free

              visgeo "visGeo_LeftHandMesh.obj"
            }
          }
        }
      }
    }
  }
}


end

