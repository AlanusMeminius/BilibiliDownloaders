#pragma once

#include <QString>
#include <QVariant>

struct sqlite3;
struct sqlite3_stmt;

class SQLiteDatabase
{
private:
    SQLiteDatabase(const SQLiteDatabase& other) = delete;
    SQLiteDatabase& operator=(const SQLiteDatabase& other) = delete;

public:
    explicit SQLiteDatabase(const QString& path);
    virtual ~SQLiteDatabase();

    bool isOpen() const;

    QStringList tables();
    QStringList views();

    bool prepare(const QString& sql);
    bool next();

    bool execute(const QString& sql);

    QVariant value(int index) const;

    QString lastError() const;

    sqlite3* handle() const;

private:
    void close();
    void finalize();
    void updateLastError();

    sqlite3* m_db = nullptr;
    sqlite3_stmt* m_stmt = nullptr;
    QString m_lastError;
};

