#ifndef SQLMETA_H
#define SQLMETA_H


class SqlMeta
{
public:
    enum SqlState {
        Selected,
        Added,
        Changed,
        Deleted,
        Invalid
    };

    SqlMeta();
    SqlMeta(int id) { id_ = id; }
    SqlMeta(int id, SqlState state) { id_ = id; state_ = state; }

    void setId(int id) { this->id_ = id; }
    void setState(SqlState state) { this->state_ = state; }
    virtual int id() { return id_; }
    SqlState state() { return state_; }

private:
    int id_ = -1;
    SqlState state_ = SqlState::Selected;
};

#endif // SQLMETA_H
