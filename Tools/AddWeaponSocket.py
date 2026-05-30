"""
Xuanming - 给 SK_Mannequin 加 WeaponSocket（M1.3）

在 UE 编辑器里执行:
  Tools -> Execute Python Script -> 选 Tools/AddWeaponSocket.py

做的事:
  1. 加载 SK_Mannequin（USkeleton 资产，UE5 Manny 命名约定）
  2. 检查是否已经存在 WeaponSocket（幂等：有就跳过，不重复加）
  3. 在 hand_r 骨骼上加一个 SkeletalMeshSocket，名字 = WeaponSocket
  4. 给一个对 FPS 持枪比较合适的相对偏移（之后可在 Editor 里 Socket 编辑器微调）
  5. 保存

C++ 端 SpawnDefaultWeapon() 里写死了 AttachToComponent(GetMesh(), ..., "WeaponSocket")，
所以名字必须就是 "WeaponSocket"。

UE5 Manny 资产命名约定:
  SK_Mannequin       = USkeleton（骨架，sockets 真正挂这里）
  SKM_Manny_Simple   = USkeletalMesh（蒙皮网格，引用 Skeleton）

参考骨骼名:
  UE5 Mannequin 的右手骨骼 = "hand_r"

经验偏移（可在 Editor 里调）:
  Location  = (X=10, Y=4, Z=-2)   往手心前方一点、稍微往拇指侧偏
  Rotation  = (Roll=0, Pitch=0, Yaw=90)  让枪管朝前（取决于武器 mesh 的 +X 方向）
"""

import unreal


# 注意：UE5 资产命名约定
#   SK_Mannequin       = USkeleton（骨架资产，sockets 挂这里）
#   SKM_Manny_Simple   = USkeletalMesh（蒙皮网格，引用上面的 Skeleton）
# WeaponSocket 必须挂在 Skeleton 上（所有用同一 Skeleton 的 Mesh 共享）。
SKELETON_PATH = "/Game/Characters/Mannequins/Meshes/SK_Mannequin"
SOCKET_NAME = "WeaponSocket"
PARENT_BONE = "hand_r"

# Socket 在父骨骼坐标系下的相对 Transform
SOCKET_LOCATION = unreal.Vector(10.0, 4.0, -2.0)
SOCKET_ROTATION = unreal.Rotator(0.0, 0.0, 90.0)
SOCKET_SCALE    = unreal.Vector(1.0, 1.0, 1.0)


def main():
    print("=" * 60)
    print(f"[Xuanming] 给 SK_Mannequin (Skeleton) 加 {SOCKET_NAME}")
    print("=" * 60)

    skeleton = unreal.EditorAssetLibrary.load_asset(SKELETON_PATH)
    if skeleton is None:
        raise RuntimeError(f"找不到 {SKELETON_PATH}")
    if not isinstance(skeleton, unreal.Skeleton):
        raise RuntimeError(
            f"{SKELETON_PATH} 不是 USkeleton（type={type(skeleton).__name__}）。"
            "UE5 Manny 命名约定: SK_Mannequin=Skeleton, SKM_Manny_Simple=SkeletalMesh"
        )
    print(f"[1/4] 加载 Skeleton: {skeleton.get_path_name()}")

    # 幂等检查：sockets 数组里是否已经有 WeaponSocket
    existing = skeleton.get_editor_property("sockets") or []
    print(f"[2/4] 现有 sockets 数: {len(existing)}")
    for s in existing:
        try:
            name = s.get_editor_property("socket_name")
        except Exception:
            name = "?"
        print(f"      - {name}")
        if str(name) == SOCKET_NAME:
            print(f"\n      已存在 {SOCKET_NAME}，跳过创建（幂等）")
            return

    # 创建新 socket
    print(f"[3/4] 创建 {SOCKET_NAME} 挂到 {PARENT_BONE}")
    socket = unreal.SkeletalMeshSocket(skeleton)
    socket.set_editor_property("socket_name", SOCKET_NAME)
    socket.set_editor_property("bone_name", PARENT_BONE)
    socket.set_editor_property("relative_location", SOCKET_LOCATION)
    socket.set_editor_property("relative_rotation", SOCKET_ROTATION)
    socket.set_editor_property("relative_scale", SOCKET_SCALE)

    # 写回 sockets 数组
    new_sockets = list(existing) + [socket]
    skeleton.set_editor_property("sockets", new_sockets)

    # 保存
    print(f"[4/4] 保存 Skeleton")
    unreal.EditorAssetLibrary.save_loaded_asset(skeleton)

    print("\n" + "=" * 60)
    print(f"[Xuanming] {SOCKET_NAME} 已加到 hand_r")
    print(f"  Location = {SOCKET_LOCATION}")
    print(f"  Rotation = {SOCKET_ROTATION}")
    print("接下来:")
    print("  1. 在 Editor 双击 SK_Mannequin 打开 Skeleton 编辑器")
    print("  2. 找到 WeaponSocket，可视化微调位置/旋转")
    print("  3. 跑 CreateWeaponBP.py 建 BP_Weapon_AK")
    print("=" * 60)


main()
