#ifndef _FSM_HPP_
#define _FSM_HPP_

#include <vector>
#include <algorithm>
#include <functional>

namespace fsm {

  /**
   * finite statemachine template
   */
  template <typename EventT = unsigned, typename StateT = unsigned>
  struct statemachine {

    /**
     * event type
     */
    typedef EventT event_t;

    /**
     * state type
     */ 
    typedef StateT state_t;

    /**
     * event filter function type
     */
    typedef std::function< bool (const event_t&) > filter_t;

    /**
     * transit callback function type
     */
    typedef std::function< void (const event_t&, const state_t&, const state_t&) > callback_t;

    /**
     * transition type
     */
    typedef struct transition {

      /**
       * this transition is only processed if the filter function returns true
       */
      const filter_t    filter;

      /**
       * this transition is only processed if the current state is equal to this state
       */
      const state_t     source;

      /**
       * after this transition has been applied the machine is in this state
       */
      const state_t     target;

      /**
       * a transit callback that is executed only if this transition is applied
       */
      const callback_t  transit;

    } transition_t;

    /**
     * transition table type
     */
    typedef std::vector<transition_t> transitions_t;

    /**
     * the current state
     */
    state_t             state;

    /**
     * the transition table
     */
    const transitions_t transitions;

    /**
     * a global transition callback
     */
    const callback_t    transit;

    /**
     * feeds a single event into the statemachine, returning true if it has been consumed
     */
    bool operator () (const event_t& event)
    {
      // for every transition...
      for (transition_t transition : transitions) {
        // check source state and event filter...
        if ((transition.source == state) && (transition.filter(event))) {
          // switch to target state
          state = transition.target;
          // handle transition's transit callback if it has been defined
          if (transition.transit) {
            transition.transit(event, transition.source, transition.target);
          }
          // handle statemachine's global transit callback if it has been defined
          if (transit) {
            transit(event, transition.source, transition.target);
          }
          // event has been consumed
          return true;
        }
      }
      // event has been ignored
      return false;
    }

    /**
     * build a callback function that is composed of calling the given callbacks in series
     */
    static inline callback_t series(const std::initializer_list<callback_t>& callbacks)
    {
      // store callbacks in a table
      std::vector<callback_t> serial(callbacks);
      // return composed callback function
      return [serial] (const event_t& evt, const state_t& src, const state_t& dst)
      {
        // run all callbacks in series...
        std::for_each(serial.begin(), serial.end(), [&evt, &src, &dst] (const callback_t& current) {
          current(evt, src, dst);
        });
      };
    }

    /**
     * build a filter function that matches any of the given event criteria
     */
    static inline filter_t match(const std::initializer_list<event_t>& critia)
    {
      // store matching events in a table
      std::vector<event_t> filter(critia);
      // return filter function that matches any of the events
      return [filter] (const event_t& evt)
      {
        // check if one of the provided events matches...
        return std::find_if(filter.begin(), filter.end(), [&evt] (const event_t& match) {
          return evt == match;
        }) != filter.end();
      };
    }

  };
  
}

#endif
