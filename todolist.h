#ifndef TODOLIST_H
#define TODOLIST_H

#include <QObject>
#include <QListWidget>

class ToDoList : public QObject
{
    Q_OBJECT

public:
    explicit ToDoList(QObject *parent = nullptr);

    // Database operations
    void loadTasksFromDB(QListWidget *todoList, QListWidget *completedList);
    void addTaskToDB(const QString &task, bool completed);
    void moveTaskInDB(const QString &task, bool fromTodoToCompleted);
    void deleteTaskFromDB(const QString &task, bool fromCompleted);

signals:
    void taskMovedToCompleted(const QString &task);
    void taskMovedToTodo(const QString &task);

private:
         // Add any private members if needed
};

#endif // TODOLIST_H
