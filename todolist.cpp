#include "todolist.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QListWidget>
#include <QListWidgetItem>

ToDoList::ToDoList(QObject *parent) : QObject(parent) {}



//---------------------------------------LOAD--------------------------------------------------------------------

void ToDoList::loadTasksFromDB(QListWidget *todoList, QListWidget *completedList) {


    todoList->clear();
    completedList->clear();

    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        qDebug() << "❌ Database not open in loadTasksFromDB()";
        return;
    }

    // Load tasks from TASKS table
    QSqlQuery taskQuery(db);
    if (!taskQuery.exec("SELECT TODO FROM TASKS")) {
        qDebug() << "❌ TASKS query error:" << taskQuery.lastError().text();
    } else {
        while (taskQuery.next()) {
            QString task = taskQuery.value(0).toString();
            todoList->addItem(new QListWidgetItem(task));
        }
        qDebug() << "✅ Loaded TODO tasks.";
    }

    // Load tasks from COMPLETED table
    QSqlQuery completedQuery(db);
    if (!completedQuery.exec("SELECT COMPLETED FROM COMPLETED")) {
        qDebug() << "❌ COMPLETED query error:" << completedQuery.lastError().text();
    } else {
        while (completedQuery.next()) {
            QString task = completedQuery.value(0).toString();
            completedList->addItem(new QListWidgetItem(task));
        }
        qDebug() << "✅ Loaded COMPLETED tasks.";
    }
}

//------------------------------------------ADD------------------------------------------

void ToDoList::addTaskToDB(const QString &task, bool completed)
{
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        qDebug() << "❌ Database not open in addTaskToDB()";
        return;
    }

    QSqlQuery query(db);
    if (completed) {
        query.prepare("INSERT INTO COMPLETED (COMPLETED) VALUES (:task)");
    } else {
        query.prepare("INSERT INTO TASKS (TODO) VALUES (:task)");
    }

    query.bindValue(":task", task);

    if (!query.exec()) {
        qDebug() << "❌ Failed to insert task:" << query.lastError().text();
    } else {
        qDebug() << "✅ Task inserted:" << task;
    }
}


//------------------Delete---------------------------------


void ToDoList::deleteTaskFromDB(const QString &task, bool fromCompleted)
{
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        qDebug() << "❌ Database not open in deleteTaskFromDB()";
        return;
    }

    QSqlQuery query(db);
    if (fromCompleted) {
        query.prepare("DELETE FROM COMPLETED WHERE COMPLETED = :task");
    } else {
        query.prepare("DELETE FROM TASKS WHERE TODO = :task");
    }

    query.bindValue(":task", task);

    if (!query.exec()) {
        qDebug() << "❌ Failed to delete task:" << query.lastError().text();
    } else {
        qDebug() << "✅ Task deleted:" << task;
    }
}



//-------------Move---------------------------------
void ToDoList::moveTaskInDB(const QString &task, bool fromTodoToCompleted) {
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        qDebug() << "❌ Database not open in moveTaskInDB()";
        return;
    }

    db.transaction();

    try {
        // Delete from source table
        QSqlQuery deleteQuery(db);
        if (fromTodoToCompleted) {
            deleteQuery.prepare("DELETE FROM TASKS WHERE TODO = :task");
        } else {
            deleteQuery.prepare("DELETE FROM COMPLETED WHERE COMPLETED = :task");
        }
        deleteQuery.bindValue(":task", task);

        if (!deleteQuery.exec()) {
            throw deleteQuery.lastError();
        }

        // Insert into target table
        addTaskToDB(task, fromTodoToCompleted);

        db.commit();
        qDebug() << "✅ Moved task:" << task;

    } catch (QSqlError &error) {
        db.rollback();
        qDebug() << "❌ Failed to move task:" << error.text();
    }
}

