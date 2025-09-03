// find_icons.cpp

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QDirIterator>
#include <QFileInfo>
#include <QIcon>
#include <QStandardPaths>
#include <QSvgRenderer>
#include <QImage>
#include <QPixmap>
#include <QPainter>
#include <QStyle>

static const QStringList allowedExts = { "png", "svg", "xpm", "ico", "bmp" };

void scanFilesystemRecursive(const QString &root, const QString &iconName, QVector<QString> &out) {
    if (!QDir(root).exists()) return;
    QStringList filters;
    for (const QString &e : allowedExts) filters << QString("*.%1").arg(e);
    QDirIterator it(root, filters, QDir::Files | QDir::NoSymLinks, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        QString path = it.next();
        if (QFileInfo(path).baseName().compare(iconName, Qt::CaseInsensitive) == 0)
            out.append(path);
    }
}

void scanResourcesRecursive(const QString &iconName, QVector<QString> &out) {
    // iterate over the resource tree (":/") recursively
    QStringList filters;
    for (const QString &e : allowedExts) filters << QString("*.%1").arg(e);
    QDirIterator it(":/", filters, QDir::Files | QDir::NoSymLinks, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        QString path = it.next();
        // debug looping resource files
        /*
        if (
            // !path.startsWith(":/icons/breeze/") &&
            !path.startsWith(QString(":/icons/%1/").arg(QIcon::themeName())) &&
            !path.startsWith(":/qt-project.org/") &&
            true
        ) {
            qDebug() << "scanResourcesRecursive: path = " << path;
        }
        */
        if (QFileInfo(path).baseName().compare(iconName, Qt::CaseInsensitive) == 0)
            out.append(path);
    }
}

QImage loadCandidateImage(const QString &path, int size) {
    QString suf = QFileInfo(path).suffix().toLower();
    if (suf == "svg") {
        QSvgRenderer renderer(path);
        if (!renderer.isValid()) return QImage();
        QImage img(size, size, QImage::Format_ARGB32);
        img.fill(Qt::transparent);
        QPainter p(&img);
        renderer.render(&p);
        return img;
    } else {
        QImage img;
        if (!img.load(path)) return QImage();
        if (img.size() != QSize(size, size))
            img = img.scaled(size, size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        return img;
    }
}

int main(int argc, char **argv) {
    QApplication app(argc, argv);

    QString iconName = "koi_tray";
    bool doCompare = false;
    for (int i = 1; i < argc; ++i) {
        QString a = QString::fromLocal8Bit(argv[i]);
        if (a == "--compare") doCompare = true;
        else iconName = a;
    }

    qDebug() << "Searching for icon name:" << iconName;
    qDebug() << "QIcon theme name:" << QIcon::themeName();
    qDebug() << "QIcon theme search paths:";
    for (auto path : QIcon::themeSearchPaths()) {
        qDebug().noquote().nospace() << "  " << path;
    }

    // Build a list of candidate filesystem roots:
    QStringList roots = QIcon::themeSearchPaths();

    // Add common extra dirs and standard locations (no duplicates)
    QStringList extras = {
        "/usr/share/icons",
        "/usr/local/share/icons",
        "/usr/share/pixmaps",
    };
    for (const QString &loc : QStandardPaths::standardLocations(QStandardPaths::GenericDataLocation)) {
        extras << QDir(loc).filePath("icons");
        extras << QDir(loc).filePath("pixmaps");
    }
    for (const QString &p : extras)
        if (!p.isEmpty() && !roots.contains(p)) roots << p;

    // Search filesystem recursively
    QVector<QString> found;
    for (const QString &root : roots) {
        scanFilesystemRecursive(root, iconName, found);
    }

    // Search resources (qrc)
    scanResourcesRecursive(iconName, found);

    if (found.isEmpty()) {
        qDebug() << "No matching files found in theme paths or resources.";
    } else {
        qDebug() << "Found candidates:";
        for (const QString &f : found) qDebug() << " -" << f;
    }

    if (doCompare) {
        qDebug() << "\nComparing to QIcon::fromTheme(...) pixmap:";
        QIcon themeIcon = QIcon::fromTheme(iconName);
        qDebug() << "  fromTheme().isNull() =" << themeIcon.isNull();
        qDebug() << "  available sizes =" << themeIcon.availableSizes();

        int compareSize = QApplication::style()->pixelMetric(QStyle::PM_SmallIconSize);
        qDebug() << "  using compare size =" << compareSize;

        QPixmap themePixmap = themeIcon.pixmap(QSize(compareSize, compareSize));
        QImage themeImage = themePixmap.toImage();
        if (themeImage.isNull()) qDebug() << "  (theme produced no pixmap at that size)";

        for (const QString &candidate : found) {
            QImage cand = loadCandidateImage(candidate, compareSize);
            if (cand.isNull()) {
                qDebug() << "  cannot load candidate (or invalid):" << candidate;
                continue;
            }
            if (themeImage == cand) {
                qDebug() << "  MATCH:" << candidate;
            } else {
                // quick pixel-equality metric
                QImage a = themeImage.convertToFormat(QImage::Format_ARGB32);
                QImage b = cand.convertToFormat(QImage::Format_ARGB32);
                int w = qMin(a.width(), b.width()), h = qMin(a.height(), b.height());
                if (w == 0 || h == 0) {
                    qDebug() << "  DIFFER:" << candidate << "(size mismatch)";
                    continue;
                }
                qint64 same = 0, total = 0;
                for (int y = 0; y < h; ++y) {
                    const QRgb *pa = reinterpret_cast<const QRgb*>(a.scanLine(y));
                    const QRgb *pb = reinterpret_cast<const QRgb*>(b.scanLine(y));
                    for (int x = 0; x < w; ++x) {
                        if (pa[x] == pb[x]) ++same;
                        ++total;
                    }
                }
                double pct = (total ? (100.0 * same / total) : 0.0);
                qDebug().nospace() << "  DIFFER: " << candidate << " -> " << pct << "% pixels equal";
            }
        }
    }

    return 0;
}
