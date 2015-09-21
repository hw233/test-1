#pragma once
#ifndef __TASK_H__
#define __TASK_H__
#include"Config.h"
#include"Player.h"
#include"Var.h"

namespace GObject
{
    class Player;
    class Var;

    class Task
    {
        public:
            Task(UInt8 t,UInt16 i,std::string s) : type(t), id(i) , name(s) {}
            UInt8 GetType() const { return type;}
            UInt16 GetId() const { return id;}
            std::string GetName() const { return name;}
        private:
            UInt8  type;  //1 每日任务  2 每周任务  3 一次性任务
            UInt16 id;
            std::string name;
    };

    class TaskManager
    {
        public:
            TaskManager(Player* player): m_owner(player) {}
            bool IsAccept(UInt8 type, UInt16 taskId);
            bool IsComplete(UInt8 type,UInt16 taskId);
            void CompleteTask(UInt8 type,UInt16 taskId);
        private:
            Player* m_owner;
    };
}

#endif
