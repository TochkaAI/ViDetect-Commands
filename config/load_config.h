#pragma once

#include "shared/config/yaml_config.h"
#include "../graphics.h"
#include "../roi.h"

namespace pproto {

using namespace std;

namespace graph {

bool loadIdentity(const YamlConfig* conf, const YAML::Node& baseNode,
                  const string& nodeName, data::Identity& identity);

bool loadColor(const YamlConfig* conf, const YAML::Node& baseBase,
               const string& nodeName, data::graph::Color& color);

bool loadBrush(const YamlConfig* conf, const YAML::Node& baseNode,
               const string& nodeName, data::graph::Brush& brush);

bool loadPoint(const YamlConfig* conf, const YAML::Node& baseNode,
               const string& nodeName, data::graph::Point& point);

bool loadSize(const YamlConfig* conf, const YAML::Node& baseNode,
              const string& nodeName, data::graph::Size& size);

bool loadOffset(const YamlConfig* conf, const YAML::Node& baseNode,
                const string& nodeName, data::graph::Offset& offset);

bool loadCaption(const YamlConfig* conf, const YAML::Node& baseNode,
                 const string& nodeName, data::graph::Caption& caption);

bool loadLine(const YamlConfig* conf, const YAML::Node& baseNode,
              const string& nodeName, data::graph::Line& line);

bool loadPolyline(const YamlConfig* conf, const YAML::Node& baseNode,
                  const string& nodeName, data::graph::Polyline& polyline);

bool loadRectangle(const YamlConfig* conf, const YAML::Node& baseNode,
                   const string& nodeName, data::graph::Rectangle& rect);

bool loadCircle(const YamlConfig* conf, const YAML::Node& baseNode,
                const string& nodeName, data::graph::Circle& circle);

} // namespace graph

bool loadRoi(const YamlConfig* conf, const YAML::Node& baseNode,
             const string& nodeName, data::Roi& roi);

} // namespace pproto
