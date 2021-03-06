#include <QtTest/QtTest>
#include <memory>

#include <TreeDS/node/multiple_node_pointer.hpp>
#include <TreeDS/node/navigator/generative_navigator.hpp>
#include <TreeDS/tree>

#include "Types.hpp"

using namespace std;
using namespace md;

class GenerativeNavigatorTest : public QObject {

    Q_OBJECT

    private slots:
    void binary1();
    void binary2();
    void nary1();
    void breadthFirstIterate();
};

void GenerativeNavigatorTest::binary1() {
    binary_tree<int> target {
        n(1)(
            n(2)(
                n(4),
                n(5)(
                    n(),
                    n(6))),
            n(3))};
    binary_tree<int> generated {n(1)};
    multiple_node_pointer ptrs(target.root().get_raw_node(), generated.root().get_raw_node());
    std::allocator<binary_node<int>> alloc;
    generative_navigator nav(
        ptrs,
        [](auto&&) {
            return true;
        },
        alloc);

    ptrs = nav.get_left_child(ptrs); // 2
    generated.update_size_arity();
    QCOMPARE(generated, n(1)(n(2)));

    ptrs = nav.get_next_sibling(ptrs); // 3
    generated.update_size_arity();
    QCOMPARE(generated, n(1)(n(2), n(3)));

    ptrs = nav.get_parent(ptrs); // 1
    generated.update_size_arity();
    QCOMPARE(generated, n(1)(n(2), n(3)));

    ptrs = nav.get_first_child(ptrs); // 2
    generated.update_size_arity();
    QCOMPARE(generated, n(1)(n(2), n(3)));

    ptrs = nav.get_last_child(ptrs); // 5
    generated.update_size_arity();
    QCOMPARE(
        generated,
        n(1)(
            n(2)(
                n(),
                n(5)),
            n(3)));

    ptrs = nav.get_first_child(ptrs); // 6
    generated.update_size_arity();
    QCOMPARE(
        generated,
        n(1)(
            n(2)(
                n(),
                n(5)(
                    n(),
                    n(6))),
            n(3)));

    ptrs = nav.get_parent(nav.get_parent(ptrs)); // 2
    generated.erase(
        std::find(
            generated.begin(policy::post_order()),
            generated.end(policy::post_order()),
            5));
    QCOMPARE(generated, n(1)(n(2), n(3)));

    ptrs = nav.get_right_child(ptrs); // 5
    generated.update_size_arity();
    QCOMPARE(
        generated,
        n(1)(
            n(2)(
                n(),
                n(5)),
            n(3)));
}

void GenerativeNavigatorTest::binary2() {
    binary_tree<int> target {
        n(-1)(
            n(-2)(
                n(-4)),
            n(3)(
                n(5)(
                    n(7),
                    n(8)(
                        n(9)(
                            n(),
                            n(11)),
                        n(-10)(
                            n(-12)))),
                n(6)))};
    binary_tree<int> generated {n(3)};
    multiple_node_pointer ptrs(target.root().go_last_child().get_raw_node(), generated.root().get_raw_node());
    std::allocator<binary_node<int>> alloc;
    generative_navigator nav(
        ptrs,
        [](auto node) {
            return node->get_value() >= 0;
        },
        alloc);

    ptrs = nav.get_right_child(ptrs); // 6
    generated.update_size_arity();
    QCOMPARE(generated, n(3)(n(), n(6)));

    ptrs = nav.get_prev_sibling(ptrs); // 5
    generated.update_size_arity();
    QCOMPARE(generated, n(3)(n(5), n(6)));

    ptrs = nav.get_last_child(ptrs); // 8
    generated.update_size_arity();
    QCOMPARE(
        generated,
        n(3)(
            n(5)(
                n(),
                n(8)),
            n(6)));

    ptrs = nav.get_last_child(ptrs); // 9
    generated.update_size_arity();
    QCOMPARE(
        generated,
        n(3)(
            n(5)(
                n(),
                n(8)(
                    n(9))),
            n(6)));

    auto other_ptr = nav.get_next_sibling(ptrs); // null
    generated.update_size_arity();
    QVERIFY(!other_ptr);
    QCOMPARE(
        generated,
        n(3)(
            n(5)(
                n(),
                n(8)(
                    n(9))),
            n(6)));
}

void GenerativeNavigatorTest::nary1() {
}

template <typename TargetPtr, typename GeneratedPtr, typename Predicate, typename Allocator>
auto create_breadth_first_generative_iterator(
    TargetPtr target,
    GeneratedPtr generated,
    Predicate&& predicate,
    Allocator&& allocator) {
    multiple_node_pointer roots(target, generated);
    generative_navigator nav(roots, predicate, allocator);
    return policy::breadth_first().get_instance(roots, nav, allocator);
}

void GenerativeNavigatorTest::breadthFirstIterate() {
    binary_tree<char> target {
        n('a')(
            n('1')(
                n(),
                n('2')(
                    n('3')(
                        n('d'),
                        n('5')),
                    n('4')(
                        n('6')(
                            n('e'),
                            n('f')),
                        n('7')))),
            n('b')(
                n('c')))};
    binary_tree<char> generated {n('1')};
    std::allocator<binary_node<char>> al;

    auto it = create_breadth_first_generative_iterator(
        target.root().go_first_child().get_raw_node(),
        generated.root().get_raw_node(),
        [](auto multiplePointer) {
            return multiplePointer->get_value() >= '1' && multiplePointer->get_value() <= '9';
        },
        std::allocator<binary_node<char>>());

    generated.update_size_arity();
    QCOMPARE(generated, n('1')(n(), n('2')));

    it.increment();
    generated.update_size_arity();
    QCOMPARE(
        generated,
        n('1')(
            n(),
            n('2')(
                n('3'))));

    it.increment();
    generated.update_size_arity();
    QCOMPARE(
        generated,
        n('1')(
            n(),
            n('2')(
                n('3')(
                    n(),
                    n('5')),
                n('4'))));

    do {
        it.increment();
    } while (it.get_current_node());
    generated.update_size_arity();
    QCOMPARE(
        generated,
        n('1')(
            n(),
            n('2')(
                n('3')(
                    n(),
                    n('5')),
                n('4')(
                    n('6'),
                    n('7')))));
}

QTEST_MAIN(GenerativeNavigatorTest);
#include "GenerativeNavigatorTest.moc"
