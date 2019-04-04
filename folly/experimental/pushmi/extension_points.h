/*
 * Copyright 2018-present Facebook, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#pragma once

#include <future>

#include <folly/experimental/pushmi/detail/awaitable_concepts.h>
#include <folly/experimental/pushmi/detail/functional.h>
#include <folly/experimental/pushmi/detail/if_constexpr.h>
#include <folly/experimental/pushmi/detail/sender_concepts.h>
#include <folly/experimental/pushmi/forwards.h>
#include <folly/experimental/pushmi/properties.h>
#include <folly/experimental/pushmi/tags.h>
#include <folly/experimental/pushmi/traits.h>
#include <folly/CppAttributes.h>
#include <folly/Portability.h>
#include <folly/lang/CustomizationPoint.h>

namespace folly {
namespace pushmi {
namespace _adl {

//
// support methods on a class reference
//

PUSHMI_TEMPLATE(class S)
(requires //
 requires(std::declval<S&>().done())) //
void set_done(S& s) //
    noexcept(noexcept((s).done())) {
  s.done();
}
PUSHMI_TEMPLATE(class S, class E)
(requires //
 requires(std::declval<S&>().error(std::declval<E>()))) //
void set_error(S& s, E e) //
    noexcept(noexcept((s).error(std::move(e)))) {
  s.error(std::move(e));
}
PUSHMI_TEMPLATE(class S, class... VN)
(requires //
 requires(std::declval<S&>().value(std::declval<VN>()...))) //
void set_value(S& s, VN&&... vn) //
    noexcept(noexcept(s.value((VN &&) vn...))) {
  s.value((VN &&) vn...);
}

PUSHMI_TEMPLATE(class S, class Up)
(requires //
 requires(std::declval<S&>().starting(std::declval<Up>()))) //
void set_starting(S& s, Up&& up) //
    noexcept(noexcept(s.starting((Up &&) up))) {
  s.starting((Up &&) up);
}

PUSHMI_TEMPLATE(class SD)
(requires //
 requires(std::declval<SD&>().executor())) //
auto get_executor(SD& sd) //
    noexcept(noexcept(sd.executor())) {
  return sd.executor();
}

PUSHMI_TEMPLATE(class SD)
(requires //
 requires(std::declval<SD&>().make_strand())) //
auto make_strand(SD& sd) //
    noexcept(noexcept(sd.make_strand())) {
  return sd.make_strand();
}

// PUSHMI_TEMPLATE(class SD, class Out)
// (requires //
//  requires(std::declval<SD>().submit(std::declval<Out>()))) //
// void submit(SD&& sd, Out&& out) //
//     noexcept(noexcept(((SD &&) sd).submit((Out &&) out))) {
//   ((SD &&) sd).submit((Out &&) out);
// }

PUSHMI_TEMPLATE(class SD)
(requires //
 requires(std::declval<SD&>().top())) //
auto top(SD& sd) //
    noexcept(noexcept(sd.top())) {
  return sd.top();
}

PUSHMI_TEMPLATE(class SD)
(requires //
 requires(std::declval<SD&>().schedule())) //
auto schedule(SD& sd) //
    noexcept(noexcept(sd.schedule())) {
  return sd.schedule();
}

PUSHMI_TEMPLATE(class SD, class TP)
(requires //
 requires(std::declval<SD&>().schedule(
     std::declval<TP (&)(TP)>()(top(std::declval<SD&>()))))) //
auto schedule(SD& sd, TP tp) //
    noexcept(noexcept(sd.schedule(std::move(tp)))) {
  return sd.schedule(std::move(tp));
}

//
// support methods on a class pointer
//

PUSHMI_TEMPLATE(class S)
(requires //
 requires(set_done(*std::declval<S>()))) //
void set_done(S&& s) //
    noexcept(noexcept(set_done(*s))) {
  set_done(*s);
}
PUSHMI_TEMPLATE(class S, class E)
(requires //
 requires(set_error(*std::declval<S>(), std::declval<E>()))) //
void set_error(S&& s, E e) //
    noexcept(noexcept(set_error(*s, std::move(e)))) {
  set_error(*s, std::move(e));
}
PUSHMI_TEMPLATE(class S, class... VN)
(requires //
 requires(set_value(*std::declval<S>(), std::declval<VN>()...))) //
void set_value(S&& s, VN&&... vn) //
    noexcept(noexcept(set_value(*s, (VN &&) vn...))) {
  set_value(*s, (VN &&) vn...);
}

PUSHMI_TEMPLATE(class S, class Up)
(requires //
 requires(set_starting(*std::declval<S>(), std::declval<Up>()))) //
void set_starting(S&& s, Up&& up) //
    noexcept(noexcept(set_starting(*s, (Up &&) up))) {
  set_starting(*s, (Up &&) up);
}

PUSHMI_TEMPLATE(class SD)
(requires //
 requires(get_executor(*std::declval<SD>()))) //
auto get_executor(SD&& sd) //
    noexcept(noexcept(get_executor(*sd))) {
  return get_executor(*sd);
}

PUSHMI_TEMPLATE(class SD)
(requires //
 requires(make_strand(*std::declval<SD>()))) //
auto make_strand(SD&& sd) //
    noexcept(noexcept(make_strand(*sd))) {
  return make_strand(*sd);
}

// PUSHMI_TEMPLATE(class SD, class Out)
// (requires //
//  requires(submit(*std::declval<SD>(), std::declval<Out>()))) //
// void submit(SD&& sd, Out&& out, ...) // MSVC(use ... to disambiguate)
//     noexcept(noexcept(submit(*sd, (Out &&) out))) {
//   submit(*sd, (Out &&) out);
// }

PUSHMI_TEMPLATE(class SD)
(requires //
 requires(top(*std::declval<SD>()))) //
auto top(SD&& sd) //
    noexcept(noexcept(top(*sd))) {
  return top(*sd);
}

PUSHMI_TEMPLATE(class SD)
(requires //
 requires(schedule(*std::declval<SD>()))) //
auto schedule(SD&& sd) //
    noexcept(noexcept(schedule(*sd))) {
  return schedule(*sd);
}

PUSHMI_TEMPLATE(class SD, class TP)
(requires //
 requires(schedule(
     *std::declval<SD>(),
     std::declval<TP (&)(TP)>()(top(std::declval<SD&>()))))) //
auto schedule(SD&& sd, TP tp) //
    noexcept(noexcept(schedule(*sd, std::move(tp)))) {
  return schedule(*sd, std::move(tp));
}

//
// support a nullary function as a StrandFactory
//

PUSHMI_TEMPLATE(class S)
(requires Invocable<S&>) //
auto make_strand(S& s) //
    noexcept(noexcept(s())) {
  return s();
}

//
// add support for std::promise externally
//

// std::promise does not support the done signal.
// either set_value or set_error must be called
template <class T>
void set_done(std::promise<T>&) noexcept {}

template <class T>
void set_error(std::promise<T>& p, std::exception_ptr e) noexcept {
  p.set_exception(std::move(e));
}
template <class T, class E>
void set_error(std::promise<T>& p, E e) noexcept {
  p.set_exception(std::make_exception_ptr(std::move(e)));
}
PUSHMI_TEMPLATE(class T, class U)
(requires ConvertibleTo<U, T>) //
void set_value(std::promise<T>& p, U&& u) //
    noexcept(noexcept(p.set_value((U &&) u))) {
  p.set_value((U &&) u);
}
inline void set_value(std::promise<void>& p) //
    noexcept(noexcept(p.set_value())) {
  p.set_value();
}

//
// accessors for free functions in this namespace
//

struct set_done_fn {
  PUSHMI_TEMPLATE(class S)
  (requires //
   requires(
       set_done(std::declval<S&>()),
       set_error(std::declval<S&>(), std::current_exception()))) //
  void operator()(S& s) const noexcept {
#if FOLLY_HAS_EXCEPTIONS
    try {
      set_done(s);
    } catch (...) {
      set_error(s, std::current_exception());
    }
#else // FOLLY_HAS_EXCEPTIONS
    set_done(s);
#endif // FOLLY_HAS_EXCEPTIONS
  }
};
struct set_error_fn {
  PUSHMI_TEMPLATE(class S, class E)
  (requires //
   requires(set_error(std::declval<S&>(), std::declval<E>()))) //
  void operator()(S& s, E&& e) const //
      noexcept(noexcept(set_error(s, (E &&)e))) {
    set_error(s, (E &&)e);
  }
};
struct set_value_fn {
  PUSHMI_TEMPLATE(class S, class... VN)
  (requires //
   requires(
       set_value(std::declval<S&>(), std::declval<VN>()...),
       set_error(std::declval<S&>(), std::current_exception()))) //
  void operator()(S& s, VN&&... vn) const noexcept {
#if FOLLY_HAS_EXCEPTIONS
    try {
      set_value(s, (VN &&) vn...);
    } catch (...) {
      set_error(s, std::current_exception());
    }
#else // FOLLY_HAS_EXCEPTIONS
    set_value(s, (VN &&) vn...);
#endif // FOLLY_HAS_EXCEPTIONS
  }
};

struct set_starting_fn {
  PUSHMI_TEMPLATE(class S, class Up)
  (requires //
   requires(
       set_starting(std::declval<S&>(), std::declval<Up>()),
       set_error(std::declval<S&>(), std::current_exception()))) //
  void operator()(S& s, Up&& up) const noexcept {
#if FOLLY_HAS_EXCEPTIONS
    try {
      set_starting(s, (Up &&) up);
    } catch (...) {
      set_error(s, std::current_exception());
    }
#else // FOLLY_HAS_EXCEPTIONS
    set_starting(s, (Up &&) up);
#endif // FOLLY_HAS_EXCEPTIONS
  }
};

struct get_executor_fn {
  PUSHMI_TEMPLATE(class SD)
  (requires requires(get_executor(std::declval<SD&>()))) //
  auto operator()(SD& sd) const //
      noexcept(noexcept(get_executor(sd))) {
    return get_executor(sd);
  }
};

struct make_strand_fn {
  PUSHMI_TEMPLATE(class SD)
  (requires requires(make_strand(std::declval<SD&>()))) //
  auto operator()(SD& sd) const //
      noexcept(noexcept(make_strand(sd))) {
    return make_strand(sd);
  }
};

// struct do_submit_fn {
//   PUSHMI_TEMPLATE(class SD, class Out)
//   (requires requires(submit(std::declval<SD>(), std::declval<Out>()))) //
//   void operator()(SD&& s, Out&& out) const //
//       noexcept(noexcept(submit((SD &&) s, (Out &&) out))) {
//     submit((SD &&) s, (Out &&) out);
//   }
// };

struct do_schedule_fn {
  PUSHMI_TEMPLATE(class SD, class... VN)
  (requires //
   requires(schedule(std::declval<SD&>(), std::declval<VN>()...))) //
  auto operator()(SD& s, VN&&... vn) const //
      noexcept(noexcept(schedule(s, (VN &&) vn...))) {
    return schedule(s, (VN &&) vn...);
  }
};

struct get_top_fn {
  PUSHMI_TEMPLATE(class SD)
  (requires //
   requires(top(std::declval<SD&>()))) //
  auto operator()(SD& sd) const //
      noexcept(noexcept(top(sd))) {
    return top(sd);
  }
};

} // namespace _adl

FOLLY_DEFINE_CPO(_adl::set_done_fn, set_done)
FOLLY_DEFINE_CPO(_adl::set_error_fn, set_error)
FOLLY_DEFINE_CPO(_adl::set_value_fn, set_value)
FOLLY_DEFINE_CPO(_adl::set_starting_fn, set_starting)
FOLLY_DEFINE_CPO(_adl::get_executor_fn, get_executor)
FOLLY_DEFINE_CPO(_adl::make_strand_fn, make_strand)
//FOLLY_DEFINE_CPO(_adl::do_submit_fn, submit)
FOLLY_DEFINE_CPO(_adl::do_schedule_fn, schedule)
FOLLY_DEFINE_CPO(_adl::get_top_fn, now)
FOLLY_DEFINE_CPO(_adl::get_top_fn, top)

/// A std::promise is a valid Receiver:
template <class T>
struct receiver_traits<std::promise<T>> {
  using receiver_category = receiver_tag;
};
template <>
struct receiver_traits<std::promise<void>> {
  using receiver_category = receiver_tag;
};

namespace _submit_adl
{
  template <class S, class R>
  void submit(S&&, R&&) = delete;

  template <class T>
  constexpr typename T::value_type const _v = T::value;

  PUSHMI_CONCEPT_DEF(
    template(class S, class R)
    concept HasMemberSubmit_,
      requires (S&& from, R&& to) (
        ((S&&) from).submit((R&&) to),
        // requires_<noexcept(((S&&) from).submit((R&&) to))>,
        requires_<std::is_void<decltype(((S&&) from).submit((R&&) to))>::value>
      )
  );

  PUSHMI_CONCEPT_DEF(
    template(class S, class R)
    concept HasNonMemberSubmit_,
      requires (S&& from, R&& to) (
        submit((S&&) from, (R&&) to),
        // requires_<noexcept(submit((S&&) from, (R&&) to))>,
        requires_<std::is_void<decltype(submit((S&&) from, (R&&) to))>::value>
      )
  );

#if defined(__cpp_coroutines)
  PUSHMI_CONCEPT_DEF(
    template (class A, class R)
    concept IsSubmittableAwaitable_,
      not detail::SenderLike_<A> && Awaitable<A> //&&
      //ReceiveError<R, std::exception_ptr> &&
      //ReceiveValue<R, coro::await_result_t<A>>
  );
#endif

  struct _fn
  {
  private:
#if defined(__cpp_coroutines)
    // See detail/awaitable_adapter.hpp for implementation.
    template<class A, class R>
    static void _submit_awaitable_(A awaitable, R to) noexcept;
#endif

    struct _impl_ {
      PUSHMI_TEMPLATE(class S, class R)
      (requires Sender<S>) // && Receiver<R>)
      auto operator()(S&& from, R&& to) const
      {
        // Prefer a .submit() member if it exists:
        PUSHMI_IF_CONSTEXPR_RETURN((HasMemberSubmit_<S, R>) (
          id((S&&) from).submit((R&&) to);
          return std::true_type{};
        ) else (
          // Otherwise, dispatch to a submit() free function if it exists:
          PUSHMI_IF_CONSTEXPR_RETURN((HasNonMemberSubmit_<S, R>) (
            submit(id((S&&) from), (R&&) to);
            return std::true_type{};
          ) else (
#if defined(__cpp_coroutines)
            // Otherwise, if we support coroutines and S looks like an
            // awaitable, dispatch to the
            PUSHMI_IF_CONSTEXPR_RETURN((IsSubmittableAwaitable_<S, R>) (
              _submit_awaitable_(id((S&&) from), (R&&) to);
              return std::true_type{};
            ) else (
              return std::false_type{};
            ))
#else
            return std::false_type{};
#endif
          ))
        ))
      }
    };

  public:
    PUSHMI_TEMPLATE(class S, class R)
    (requires Invocable<_impl_, S, R> && invoke_result_t<_impl_, S, R>::value)
    constexpr void operator()(S&& from, R&& to) const {
      (void) _impl_{}((S&&) from, (R&&) to);
    }
  };

#if defined(__cpp_coroutines)
  struct FOLLY_MAYBE_UNUSED oneway_task
  {
    struct promise_type
    {
      oneway_task get_return_object() noexcept { return {}; }
      std::experimental::suspend_never initial_suspend() noexcept { return {}; }
      std::experimental::suspend_never final_suspend() noexcept { return {}; }
      void return_void() noexcept {}
      [[noreturn]] void unhandled_exception() noexcept { std::terminate(); }
    };
  };
#endif

#if defined(__cpp_coroutines)
  // Make all awaitables senders:
  template<class A, class R>
  void _fn::_submit_awaitable_(A awaitable, R to) noexcept
  {
    // TRICKY: We want to make sure that if copying/moving 'awaitable' throws
    // or if allocating the coroutine frame throws that we can still call
    // op::set_error() on the receiver. So we pass the receiver by reference
    // so that if the call to std::invoke() throws that we can catch the
    // exception and pass it into 'receiver'.
#if FOLLY_HAS_EXCEPTIONS
    try
    {
#endif
      // Create a lambda coroutine and immediately invoke it:
      [](A a, R&& r) -> oneway_task
      {
        // Receivers should be nothrow move-constructible so we should't need to
        // worry about this potentially throwing.
        R rCopy(static_cast<R&&>(r));
#if FOLLY_HAS_EXCEPTIONS
        try
        {
#endif
          PUSHMI_IF_CONSTEXPR ((std::is_void<coro::await_result_t<A>>::value) (
            co_await static_cast<A&&>(a);
            set_value(id(rCopy));
          ) else (
            set_value(id(rCopy), co_await static_cast<A&&>(a));
          ))
          set_done(rCopy);
#if FOLLY_HAS_EXCEPTIONS
        }
        catch (...)
        {
          set_error(rCopy, std::current_exception());
        }
#endif
      }(static_cast<A&&>(awaitable), static_cast<R&&>(to));
#if FOLLY_HAS_EXCEPTIONS
    }
    catch (...)
    {
      set_error(to, std::current_exception());
    }
#endif
  }
#endif
} // namespace _submit_adl

FOLLY_DEFINE_CPO(_submit_adl::_fn, submit)

} // namespace pushmi
} // namespace folly
