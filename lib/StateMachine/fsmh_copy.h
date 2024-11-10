// #pragma once

// #include <vector>
// #include <optional>

// namespace fsm
// {
//     template <typename E, typename S, typename A>
//     struct Transition
//     {
//         S state;
//         std::optional<E> event{};
//         std::optional<A> action{};
//         std::optional<S> newState{};
//     };

//     template <typename E, typename S, typename A>
//     struct Fsm
//     {
//         Fsm(S initialState, std::vector<fsm::Transition<E, S, A>> &trans)
//             : currentState(initialState), transitions(trans) {};

//         S currentState;
//         std::vector<fsm::Transition<E, S, A>> transitions;

//         std::optional<A> input(E event)
//         {
//             for (auto &transition : transitions)
//             {
//                 if (transition.state == currentState && (!transition.event.has_value() || transition.event.value() == event))
//                 {
//                     if (transition.newState.has_value())
//                     {
//                         currentState = transition.newState.value();
//                     }
//                     return transition.action;
//                 }
//             }
//             return std::nullopt;
//         }
//     };

// }; // namespace fsm

// // ----------------
// // Example usage:
// // ----------------

// namespace elevator
// {

//     enum State
//     {
//         Top,
//         Bottom,
//     };

//     enum Action
//     {
//         MoveUp,
//         MoveDown,
//     };

//     enum Event
//     {
//         ButtonUp,
//         ButtonDown,
//     };

//     std::vector<fsm::Transition<Event, State, Action>> transitions{
//         {.state = Top, .event = ButtonDown, .action = MoveDown, .newState = Bottom},
//         {.state = Bottom, .event = ButtonUp, .action = MoveUp, .newState = Top},
//     };

//     auto elevatorFSM = fsm::Fsm<Event, State, Action>(Top, transitions);

// }
