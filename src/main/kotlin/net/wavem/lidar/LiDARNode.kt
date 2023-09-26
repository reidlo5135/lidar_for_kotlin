package net.wavem.lidar

class LiDARNode {
    companion object {
        init {
            System.loadLibrary("lidar_for_kotlin")
        }
    }

    external fun nativeNodeRun()
    external fun nativeRCLKotlinInit() : Long
    external fun nativeCreateNode(rclKotlinInit : Long) : Long

    external fun nativeSpin()
}

fun main(args : Array<String>) {
    val liDARNode : LiDARNode = LiDARNode()
    val rclKotlinInit : Long = liDARNode.nativeRCLKotlinInit()

    println("rclKotlinInit : $rclKotlinInit")

    val rclKotlinNode : Long = liDARNode.nativeCreateNode(rclKotlinInit)
    println("rclKotlinNode : $rclKotlinNode")

    liDARNode.nativeSpin()
}