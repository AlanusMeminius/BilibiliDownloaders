#include "UrlProcess.h"

bool util::UrlProcess::IsUrl(const QString& text)
{
    return text.startsWith("http://") || text.startsWith("https://");
}
