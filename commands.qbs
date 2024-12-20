import qbs
import QbsUtl
import ProbExt

Product {
    name: "ViDetect"

    Properties {
        condition: project.isViDetectProject === true
        name: "Commands"
    }

    targetName: "commands"
    condition: true

    type: "staticlibrary"

    Depends { name: "cpp" }
    Depends { name: "PProto" }
    Depends { name: "RapidJson"; required: false }
    Depends { name: "SharedLib" }
    Depends { name: "lib.opencv" /*; required: false*/ }
    Depends { name: "Qt"; submodules: ["core", "network"] }

//    Properties {
//        condition: lib.opencv.present
//        lib.opencv.version: project.opencvVersion
//    }

    lib.opencv.version: project.opencvVersion

//    cpp.defines: {
//        var def = project.cppDefines;
//        if (lib.opencv.present)
//            def.push("OPENCV_IS_PRESENT");
//        return def;
//    }

    cpp.defines: project.cppDefines
    cpp.cxxFlags: project.cxxFlags
    cpp.cxxLanguageVersion: project.cxxLanguageVersion

    property var includePaths: [
        "..",
    ]
    cpp.includePaths: includePaths;

    cpp.systemIncludePaths: QbsUtl.concatPaths(
        Qt.core.cpp.includePaths
       ,lib.opencv.includePath
    )

    files: [
        "error.h",
        "commands.cpp",
        "commands.h",
    ]

    Export {
        Depends { name: "cpp" }
        cpp.includePaths: exportingProduct.includePaths
    }

} // Product

