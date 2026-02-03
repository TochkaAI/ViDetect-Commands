#include "video.h"
#include "pproto/commands/pool.h"

namespace pproto {
namespace command {

#define REGISTRY_COMMAND_SINGLPROC(COMMAND, UUID) \
    const QUuidEx COMMAND = command::Pool::Registry{UUID, #COMMAND, false};

#define REGISTRY_COMMAND_MULTIPROC(COMMAND, UUID) \
    const QUuidEx COMMAND = command::Pool::Registry{UUID, #COMMAND, true};

REGISTRY_COMMAND_SINGLPROC(VideoStreamOpened, "69448105-ed53-4307-8436-a549861c56a2")
REGISTRY_COMMAND_SINGLPROC(VideoStreamClosed, "6d0ba6b5-fcec-425a-92bb-2937c725fa3e")
REGISTRY_COMMAND_SINGLPROC(VideoStreamIsOpen, "5b855d5b-d23f-488f-b237-b9b5685dfaf7")

#undef REGISTRY_COMMAND_SINGLPROC
#undef REGISTRY_COMMAND_MULTIPROC

} // namespace command

namespace data {

} // namespace data
} // namespace pproto

