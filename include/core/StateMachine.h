#pragma once
#include <memory>
#include <vector>
#include <functional>

class State;
struct Context;

class StateMachine 
{
public:
    using StatePtr = std::unique_ptr<State>;
    using Factory = std::function<StatePtr(Context)>;

    explicit StateMachine(Context ctx);

    template<class T, class... Args>
    void push(Args&&... args);

    void pop();
    void clear();
    void apply();

    void handleEvent(const sf::Event& e);
    void update(float dt);
    void render(class sf::RenderTarget& rt);

    bool empty() const { return m_stack.empty(); }

private:
    enum class OpType { Push, Pop, Clear };
    struct Op { OpType type; Factory factory; };

    std::vector<StatePtr> m_stack;
    std::vector<Op> m_ops;
    Context m_ctx;
};

template<class T, class... Args>
inline void StateMachine::push(Args&&... args) 
{
    Factory f = [=](Context c) 
        {
            return std::make_unique<T>(c, args...);
        };
    m_ops.push_back({ OpType::Push, std::move(f) });
}