#include "todolist.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QListWidget>
#include <QListWidgetItem>

ToDoList::ToDoList(QObject *parent) : QObject(parent) {}

void ToDoList::loadTasksFromDB(QListWidget *todoList, QListWidget *completedList)
{
    // Clear existing items
    todoList->clear();
    completedList->clear();

    // Load TODO items
    QSqlQuery todoQuery;
    if (!todoQuery.exec("SELECT TODO FROM TASKS")) {
        qDebug() << "Error loading TODO tasks:" << todoQuery.lastError().text();
    } else {
        while (todoQuery.next()) {
            QString task = todoQuery.value(0).toString();
            todoList->addItem(new QListWidgetItem(task));
        }
    }

    // Load COMPLETED items
    QSqlQuery completedQuery;
    if (!completedQuery.exec("SELECT COMPLETED FROM COMPLETED")) {
        qDebug() << "Error loading COMPLETED tasks:" << completedQuery.lastError().text();
    } else {
        while (completedQuery.next()) {
            QString task = completedQuery.value(0).toString();
            completedList->addItem(new QListWidgetItem(task));
        }
    }
}

void ToDoList::addTaskToDB(const QString &task, bool completed)
{
    QSqlQuery query;
    if (completed) {
        query.prepare("INSERT INTO COMPLETED (COMPLETED) VALUES (?)");
    } else {
        query.prepare("INSERT INTO TASKS (TODO) VALUES (?)");
    }
    query.addBindValue(task);

    if (!query.exec()) {
        qDebug() << "Error adding task:" << query.lastError().text();
    }
}

void ToDoList::moveTaskInDB(const QString &task, bool fromTodoToCompleted)
{
    // First delete from source table
    if (fromTodoToCompleted) {
        QSqlQuery deleteQuery;
        deleteQuery.prepare("DELETE FROM TASKS WHERE TODO = ?");
        deleteQuery.addBindValue(task);
        if (!deleteQuery.exec()) {
            qDebug() << "Error deleting from TODO:" << deleteQuery.lastError().text();
            return;
        }

        // Then add to completed
        addTaskToDB(task, true);
    } else {
        QSqlQuery deleteQuery;
        deleteQuery.prepare("DELETE FROM COMPLETED WHERE COMPLETED = ?");
        deleteQuery.addBindValue(task);
        if (!deleteQuery.exec()) {
            qDebug() << "Error deleting from COMPLETED:" << deleteQuery.lastError().text();
            return;
        }

        // Then add to todo
        addTaskToDB(task, false);
    }
}

void ToDoList::deleteTaskFromDB(const QString &task, bool fromCompleted)
{
    QSqlQuery query;
    if (fromCompleted) {
        query.prepare("DELETE FROM COMPLETED WHERE COMPLETED = ?");
    } else {
        query.prepare("DELETE FROM TASKS WHERE TODOD = ?");
    }
    query.addBindValue(task);

    if (!query.exec()) {
        qDebug() << "Error deleting task:" << query.lastError().text();
    }
}
