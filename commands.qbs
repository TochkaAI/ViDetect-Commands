import qbs
import qbs.FileInfo
import QbsUtl

Product {
    name: "ViDetect"
    targetName: "videtect"
    condition: true

    Properties {
        name: "Commands"
        targetName: "commands"
        condition: project.isViDetectProject === true
    }

    type: "staticlibrary"

    Depends { name: "cpp" }
    Depends { name: "PProto" }
    Depends { name: "Detection"; required: false }
    Depends { name: "RapidJson"; required: false }
    Depends { name: "SharedLib" }
  //Depends { name: "Video" }
    Depends { name: "Yaml" }
    Depends { name: "lib.opencv" }
    Depends { name: "Qt"; submodules: ["core", "network"] }

   // Properties {
   //     condition: lib.opencv.present
   //     lib.opencv.version: project.opencvVersion
   // }

    lib.opencv.version: project.opencvVersion

    cpp.defines: {
        var def = project.cppDefines;
        if (Detection.present)
            def.push("DETECTION_IS_PRESENT");
        return def;
    }
    cpp.cxxFlags: project.cxxFlags
    cpp.cxxLanguageVersion: project.cxxLanguageVersion

    cpp.includePaths: [".."]

    cpp.systemIncludePaths: QbsUtl.concatPaths(
        lib.opencv.includePath
    )

    files: [
        "config/load_config.cpp",
        "config/load_config.h",
        "classifier.cpp",
        "classifier.h",
        "commands.cpp",
        "commands.h",
        "compare.h",
        "detect_rect.cpp",
        "detect_rect.h",
        "error.h",
        "fpfn.cpp",
        "fpfn.h",
        "graphics.cpp",
        "graphics.h",
        "graphset.cpp",
        "graphset.h",
        "logicset.cpp",
        "logicset.h",
        "model.cpp",
        "model.h",
        "paint_rect.cpp",
        "paint_rect.h",
        "roi.cpp",
        "roi.h",
        "rois_info.cpp",
        "rois_info.h",
        "saver.cpp",
        "saver.h",
        "tracking.cpp",
        "tracking.h",
        "video.cpp",
        "video.h",
        "coordinates.cpp",
        "coordinates.h",
    ]

    Export {
        Depends { name: "cpp" }
        cpp.includePaths: [
            FileInfo.joinPaths(exportingProduct.sourceDirectory, ".."),
        ]
    }

} // Product

