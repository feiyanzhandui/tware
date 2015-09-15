#ifndef PLANNER_H
#define PLANNER_H

#include <string>

#include "data/Schema.h"
#include "expression/Expression.h"
#include "expression/Load.h"
#include "expression/Map.h"
#include "expression/Reduce.h"
#include "job/Job.h"
//#include "job/Task.h"
#include "message/AttributeMessage.pb.h"
#include "message/ExpressionMessage.pb.h"
#include "message/JobMessage.pb.h"
#include "udf/Udf.h"

using std::string;

#include <vector>
using std::vector;

namespace tupleware {
class Planner {
public:
    Planner(vector<Udf *> functs) {
        m_functs = functs;
        //m_job = new Job(1, m_functs);
        addTask();
    }

    //Job *plan(JobMessage *jobMsg) {
    vector<Expression *> plan(JobMessage *jobMsg) {
        expression(jobMsg->workflow());
        //return m_job;
        return m_exprs;
    }
private:
    vector<Udf *> m_functs;
    Job *m_job;
    //Task *m_task;
    int m_currTask;
    vector<Expression *> m_exprs;

    Expression *expression(const ExpressionMessage exprMsg) {
        switch (exprMsg.type()) {
            //case ExpressionMessage::FILTER:
            //    return filter(exprMsg);
            //case ExpressionMessage::FLAT_MAP:
            //    return flatMap(exprMsg);
            //case ExpressionMessage::GROUP:
            //    return group(exprMsg);
            //case ExpressionMessage::JOIN:
            //    return join(exprMsg);
            case ExpressionMessage::LOAD:
                return load(exprMsg);
            //case ExpressionMessage::LOOP:
            //    return loop(exprMsg);
            case ExpressionMessage::MAP:
                return map(exprMsg);
            //case ExpressionMessage::MERGE:
            //    return merge(exprMsg);
            //case ExpressionMessage::PEEK:
            //    return peek(exprMsg);
            case ExpressionMessage::REDUCE:
                return reduce(exprMsg);
            //case ExpressionMessage::SAVE:
            //    return save(exprMsg);
            //case ExpressionMessage::SORT:
            //    return sort(exprMsg);
            //case ExpressionMessage::UNION:
            //    return union(exprMsg);
            default:
                return NULL;
        }
    }
/*
    Expression *filter(const ExpressionMessage exprMsg) {
        FilterMessage ext = exprMsg.GetExtension(FilterMessage::ext);
        Expression *src = expression(ext.source());
        Function *funct = m_functs->get(ext.function());
        Expression *e =  new Filter(src, funct);
        m_job->tasks().back()->addExpression(e);
        return e;
    }

    Expression *flatMap(const ExpressionMessage expr) {
        MapMessage ext = expr.GetExtension(MapMessage::ext);
        Expression *src = expression(ext.source());
        Function *funct = m_functs->get(ext.function());

        return NULL;
    }

    Expression *group(const ExpressionMessage expr) {
        GroupMessage ext = expr.GetExtension(GroupMessage::ext);
        Expression *src = expression(ext.source());
        Function *funct = m_functs->get(ext.function());
        Function *keyFunct = m_functs->get(ext.key_function());

        return NULL;
    }

    Expression *join(const ExpressionMessage expr) {
        JoinMessage ext = expr.GetExtension(JoinMessage::ext);
        ExpressionPlan *lhs = expression(ext.left_hand_side());
        ExpressionPlan *rhs = expression(ext.right_hand_side());

        Join *join = new Join(lhs, rhs);
        return join;
    }
*/
    Expression *load(const ExpressionMessage exprMsg) {
        LoadMessage ext = exprMsg.GetExtension(LoadMessage::ext);
        string filename = ext.filename();
        Schema *sch = new Schema();
        //int numAttrs = ext.attribute_size();
        //for (int i = 0; i < numAttrs; i++)
        //    addAttribute(sch, ext.attribute(i));
        Expression *expr = new Load(filename, sch);
        //m_task->addExpression(expr);
        m_exprs.push_back(expr);
        return expr;
    }
/*
    Expression *loop(const ExpressionMessage exprMsg) {
        LoopMessage ext = exprMsg.GetExtension(LoopMessage::ext);
        Expression *src = expression(ext.source());
        Function *condition = m_functs->get(ext.invariant());
        Expression *e = new Loop(src, condition);
        m_job->tasks().back()->addExpression(e);
        return e;
    }
*/
    Expression *map(const ExpressionMessage exprMsg) {
        MapMessage ext = exprMsg.GetExtension(MapMessage::ext);
        Expression *src = expression(ext.source());
        Udf *funct = m_functs[ext.function()];
        Expression *expr = new Map(src, funct);
        //m_task->addExpression(expr);
        m_exprs.push_back(expr);
        return expr;
    }
/*
    Expression *merge(const ExpressionMessage expr) {
        MergeMessage ext = expr.GetExtension(MergeMessage::ext);
        Expression *lhs = expression(ext.left_hand_side());
        Expression *rhs = expression(ext.right_hand_side());

        Merge *mrg = new Merge(lhs, rhs);
        return NULL;
    }

    Expression *peek(const ExpressionMessage expr) {
        PeekMessage ext = expr.GetExtension(PeekMessage::ext);
        ExpressionPlan *src = expression(ext.source());
        int limit = ext.limit();

        Peek *peek = new Peek(src, limit);
        return peek;
    }
*/
    Expression *reduce(const ExpressionMessage exprMsg) {
        ReduceMessage ext = exprMsg.GetExtension(ReduceMessage::ext);
        Expression *src = expression(ext.source());
        Udf *funct = m_functs[ext.function()];
        Udf *keyFunct = m_functs[ext.key_function()];
        Expression *expr = new Reduce(src, funct, keyFunct);
        //m_task->addExpression(expr);
        m_exprs.push_back(expr);
        addTask();
        return expr;
    }
/*
    Expression *save(const ExpressionMessage exprMsg) {
        SaveMessage ext = exprMsg.GetExtension(SaveMessage::ext);
    }

    Expression *sort(const ExpressionMessage expr) {
        SortMessage ext = expr.GetExtension(SortMessage::ext);
        Expression *src = expression(ext.source());
        Function *funct = m_functs->get(ext.function());

        Sort *sort = new Sort(src, funct);
        return sort;
    }

    Expression *union(const ExpressionMessage expr) {
        UnionMessage ext = expr.GetExtension(UnionMessage::ext);
        Expression *lhs = expression(ext.left_hand_side());
        Expression *rhs = expression(ext.right_hand_side());

        Union *uni = new Union(lhs, rhs);
        return uni;
    }
*/

    void addTask() {
        //m_task = new Task();
    }
};
}
#endif
