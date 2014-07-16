#ifndef WHERE_H
#define WHERE_H

#include <QList>
#include <QVariant>
#include <QString>

namespace Orm4Qt
{
    /**
     * This enum specify the type of operation used in the condition of the where clause
     */
    enum WhereOp
    {
        Equals,
        NotEquals,
        GreaterThan,
        GreaterEqual,
        LessThan,
        LessEqual,
        Contains,
        StartsWith,
        EndsWith,
        Like,
        In,
        NotIn,
        NoOperation
    };
    /**
     * This enum specify the type of join used to combine two where instances.
     */
    enum WhereJoin
    {
        And,
        Or,
        NoJoin
    };
    /**
     * This enum specify if is necessary to open or close a block of precedence during the creation of the where clause.
     */
    enum WherePrecede
    {
        Begin,
        End,
        NoPrecede
    };
    class Where
    {
    public:
        Where(const QString &field, WhereOp op, const QList<QVariant> &arguments, WherePrecede precede=WherePrecede::NoPrecede);
        Where();
        Where(const Where &other);
        virtual ~Where();
        WhereOp operation() const;
        WhereJoin join() const;
        WherePrecede precede() const;
        Where *joined() const;
        const QString &field() const;
        const QList<QVariant> &arguments() const;
        virtual Where& And(const Where &where);
        virtual Where& Or(const Where &where);
    private:
        QString m_field;
        WhereOp m_operation;
        QList<QVariant> m_arguments;
        WhereJoin m_join;
        WherePrecede m_precede;
        Where *m_joined = nullptr;
    };
}

#endif // WHERE_H
