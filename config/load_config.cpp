#include "load_config.h"

#include "shared/break_point.h"
#include "shared/logger/logger.h"
#include "shared/logger/format.h"
#include "shared/config/appl_conf.h"
#include "shared/qt/logger_operators.h"

#define log_error_m   alog::logger().error  (alog_line_location, "LoadConfig")
#define log_warn_m    alog::logger().warn   (alog_line_location, "LoadConfig")
#define log_info_m    alog::logger().info   (alog_line_location, "LoadConfig")
#define log_verbose_m alog::logger().verbose(alog_line_location, "LoadConfig")
#define log_debug_m   alog::logger().debug  (alog_line_location, "LoadConfig")
#define log_debug2_m  alog::logger().debug2 (alog_line_location, "LoadConfig")

namespace pproto {
namespace graph {

bool loadIdentity(const YamlConfig* conf, const YAML::Node& baseNode,
                  const string& nodeName, data::graph::Identity& identity)
{
    YAML::Node node = conf->node(baseNode, nodeName);
    if (!node)
        return false;

    if (node.IsNull())
    {
        identity = data::graph::Identity();
        return true;
    }

    conf->getValue(node, "id",    identity.id,    false);
    conf->getValue(node, "index", identity.index, false);
    conf->getValue(node, "name",  identity.name , false);
    return true;
}

bool loadColor(const YamlConfig* conf, const YAML::Node& baseBase,
               const string& nodeName, data::graph::Color& color)
{
    YAML::Node node = conf->node(baseBase, nodeName);
    if (!node)
        return false;

    if (node.IsNull())
    {
        color = data::graph::Color();
        return true;
    }

    conf->getValue(node, "r", color.r, false);
    conf->getValue(node, "g", color.g, false);
    conf->getValue(node, "b", color.b, false);
    return true;
}

bool loadBrush(const YamlConfig* conf, const YAML::Node& baseNode,
               const string& nodeName, data::graph::Brush& brush)
{
    YAML::Node node = conf->node(baseNode, nodeName);
    if (!node)
        return false;

    if (node.IsNull())
    {
        brush = data::graph::Brush();
        return true;
    }

   conf->getValue  (      node, "thickness", brush.thickness, false);
   graph::loadColor(conf, node, "color"    , brush.color);
   return true;
}

bool loadPoint(const YamlConfig* conf, const YAML::Node& baseNode,
               const string& nodeName, data::graph::Point& point)
{
    YAML::Node node = conf->node(baseNode, nodeName);
    if (!node)
        return false;

    if (node.IsNull())
    {
        point = data::graph::Point();
        return true;
    }

    conf->getValue(node, "x", point.x, false);
    conf->getValue(node, "y", point.y, false);
    return true;
}

bool loadSize(const YamlConfig* conf, const YAML::Node& baseNode,
              const string& nodeName, data::graph::Size& size)
{
    YAML::Node node = conf->node(baseNode, nodeName);
    if (!node)
        return false;

    if (node.IsNull())
    {
        size = data::graph::Size();
        return true;
    }

    conf->getValue(node, "w", size.width,  false);
    conf->getValue(node, "h", size.height, false);
    return true;
}

bool loadOffset(const YamlConfig* conf, const YAML::Node& baseNode,
                const string& nodeName, data::graph::Offset& offset)
{
    YAML::Node node = conf->node(baseNode, nodeName);
    if (!node)
        return false;

    if (node.IsNull())
    {
        offset = data::graph::Offset();
        return true;
    }

    conf->getValue(node, "dx", offset.dx, false);
    conf->getValue(node, "dy", offset.dy, false);
    return true;
}

bool loadCaption(const YamlConfig* conf, const YAML::Node& baseNode,
                 const string& nodeName, data::graph::Caption& caption)
{
    YAML::Node node = conf->node(baseNode, nodeName);
    if (!node)
        return false;

    if (node.IsNull())
    {
        caption = data::graph::Caption();
        return true;
    }

    loadIdentity  (conf, node, "identity"   , caption.identity);
    conf->getValue(      node, "visible"    , caption.visible, false);
    loadBrush     (conf, node, "brush"      , caption.brush);
    loadPoint     (conf, node, "position"   , caption.position);
    loadOffset    (conf, node, "offset"     , caption.offset);
    conf->getValue(      node, "scale"      , caption.scale, false);
    conf->getValue(      node, "text"       , caption.text, false);
    conf->getValue(      node, "corner_bind", caption.cornerBind, false);

    caption.cornerBind = qBound(0, caption.cornerBind, 4);
    return true;
}

bool loadLine(const YamlConfig* conf, const YAML::Node& baseNode,
              const string& nodeName, data::graph::Line& line)
{
    YAML::Node node = conf->node(baseNode, nodeName);
    if (!node)
        return false;

    if (node.IsNull())
    {
        line = data::graph::Line();
        return true;
    }

    loadIdentity  (conf, node, "identity", line.identity);
    conf->getValue(      node, "visible" , line.visible, false);
    loadBrush     (conf, node, "brush"   , line.brush);
    loadPoint     (conf, node, "point1"  , line.point1);
    loadPoint     (conf, node, "point2"  , line.point2);
    return true;
}

bool loadPolyline(const YamlConfig* conf, const YAML::Node& baseNode,
                  const string& nodeName, data::graph::Polyline& polyline)
{
    YAML::Node node = conf->node(baseNode, nodeName);
    if (!node)
        return false;

    if (node.IsNull())
    {
        polyline = data::graph::Polyline();
        return true;
    }

    loadIdentity  (conf, node, "identity", polyline.identity);
    conf->getValue(      node, "visible" , polyline.visible, false);
    loadBrush     (conf, node, "brush"   , polyline.brush);
    conf->getValue(      node, "closed"  , polyline.closed, false);

    YamlConfig::Func loadPoints = [&](YamlConfig* conf, YAML::Node& nodes, bool)
    {
        for (const YAML::Node& node : nodes)
        {
            data::graph::Point point;
            loadPoint(conf, node, ".", point);
            polyline.points.append(point);
        }
        return true;
    };
    conf->getValue(node, "points", loadPoints, false);

    return true;
}

bool loadRectangle(const YamlConfig* conf, const YAML::Node& baseNode,
                   const string& nodeName, data::graph::Rectangle& rect)
{
    YAML::Node node = conf->node(baseNode, nodeName);
    if (!node)
        return false;

    if (node.IsNull())
    {
        rect = data::graph::Rectangle();
        return true;
    }

//    YAML::Node node1 = base["border"];
//    bool b2 = node1.IsDefined();
//    bool b1 = node1.IsNull();

//    if (node.IsNull())
//        return false;

    loadIdentity  (conf, node, "identity", rect.identity);
    conf->getValue(      node, "visible" , rect.visible, false);
    loadBrush     (conf, node, "brush"   , rect.brush);
    loadPoint     (conf, node, "point"   , rect.point1);
    loadPoint     (conf, node, "point1"  , rect.point1);
    loadPoint     (conf, node, "point2"  , rect.point2);
    loadSize      (conf, node, "size"    , rect.size);
    return true;
}

bool loadCircle(const YamlConfig* conf, const YAML::Node& baseNode,
                const string& nodeName, data::graph::Circle& circle)
{
    YAML::Node node = conf->node(baseNode, nodeName);
    if (!node)
        return false;

    if (node.IsNull())
    {
        circle = data::graph::Circle();
        return true;
    }

    loadIdentity  (conf, node, "identity", circle.identity);
    conf->getValue(      node, "visible" , circle.visible, false);
    loadBrush     (conf, node, "brush"   , circle.brush);
    loadPoint     (conf, node, "center"  , circle.center);
    conf->getValue(      node, "radius"  , circle.radius, false);
    conf->getValue(      node, "filled"  , circle.filled, false);
    return true;
}

bool loadRoi(const YamlConfig* conf, const YAML::Node& baseNode,
             const string& nodeName, data::graph::Roi& roi)
{
    YAML::Node node = conf->node(baseNode, nodeName);
    if (!node)
        return false;

    if (node.IsNull())
    {
        roi = data::graph::Roi();
        return true;
    }

    loadBrush(conf, node, "brush", roi.caption.brush);
    loadBrush(conf, node, "brush", roi.border.brush);

    loadIdentity  (conf, node, "identity", roi.identity);
    conf->getValue(      node, "visible" , roi.visible, false);
    loadCaption   (conf, node, "caption" , roi.caption);
    loadRectangle (conf, node, "border"  , roi.border);
    return true;
}

} // namespace graph
} // namespace pproto
