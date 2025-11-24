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
    Depends { name: "Yaml" }
    Depends { name: "lib.opencv" }
    Depends { name: "Qt"; submodules: ["core", "network"] }

//    Properties {
//        condition: lib.opencv.present
//        lib.opencv.version: project.opencvVersion
//    }

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
        Qt.core.cpp.includePaths // Декларация для подавления Qt warning-ов
       ,lib.opencv.includePath
    )

    files: [
        "config/load_config.cpp",
        "config/load_config.h",
        "commands.cpp",
        "commands.h",
        "error.h",
        "fpfn.cpp",
        "fpfn.h",
        "graphics.cpp",
        "graphics.h",
        "graphset.cpp",
        "graphset.h",
        "model.cpp",
        "model.h",
        "roi.cpp",
        "roi.h",
        "saver.cpp",
        "saver.h",
        "tracker.cpp",
        "tracker.h",
    ]

    Export {
        Depends { name: "cpp" }
        cpp.includePaths: [
            FileInfo.joinPaths(exportingProduct.sourceDirectory, ".."),
        ]
    }

} // Product

