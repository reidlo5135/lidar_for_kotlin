package net.wavem.lidar

class LiDARNode {
    companion object {
        init {
            System.loadLibrary("liblidar_for_kotlin")
        }
    }

    external fun nativeNodeRun()
}

fun main(args : Array<String>) {
    val liDARNode : LiDARNode = LiDARNode()
    liDARNode.nativeNodeRun()
}