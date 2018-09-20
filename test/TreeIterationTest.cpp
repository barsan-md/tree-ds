#include <QtTest/QtTest>
#include <algorithm>
#include <iterator>
#include <list>
#include <string>

#include <TreeDS/tree.hpp>

using namespace std;
using namespace ds;

Q_DECLARE_METATYPE(list<string>);
Q_DECLARE_METATYPE(binary_tree<string>);

class TreeIterationTest : public QObject {

    Q_OBJECT

    private slots:
    void iteration();
    void iteration_data();
};

void TreeIterationTest::iteration() {
    QFETCH(binary_tree<string>, t);
    QFETCH(int, expectedSize);
    QFETCH(list<string>, expectedPreOrder);
    QFETCH(list<string>, expectedInOrder);
    QFETCH(list<string>, expectedPostOrder);

    list<string> actualPreOrder;
    list<string> actualInOrder;
    list<string> actualPostOrder;
    int actualSize = static_cast<int>(t.size());

    copy(t.begin<pre_order>(), t.end<pre_order>(), back_inserter(actualPreOrder));
    copy(t.begin<in_order>(), t.end<in_order>(), back_inserter(actualInOrder));
    copy(t.begin<post_order>(), t.end<post_order>(), back_inserter(actualPostOrder));

    QCOMPARE(actualSize, expectedSize);
    QCOMPARE(actualPreOrder, expectedPreOrder);
    QCOMPARE(actualInOrder, expectedInOrder);
    QCOMPARE(actualPostOrder, expectedPostOrder);

    /*   ---   Reverse order test   ---   */
    list<string> actualReversePreOrder;
    list<string> actualReverseInOrder;
    list<string> actualReversePostOrder;

    list<string> expectedReversePreOrder;
    list<string> expectedReverseInOrder;
    list<string> expectedReversePostOrder;

    copy(t.rbegin<pre_order>(), t.rend<pre_order>(), back_inserter(actualReversePreOrder));
    copy(t.rbegin<in_order>(), t.rend<in_order>(), back_inserter(actualReverseInOrder));
    copy(t.rbegin<post_order>(), t.rend<post_order>(), back_inserter(actualReversePostOrder));

    copy(expectedPreOrder.rbegin(), expectedPreOrder.rend(), back_inserter(expectedReversePreOrder));
    copy(expectedInOrder.rbegin(), expectedInOrder.rend(), back_inserter(expectedReverseInOrder));
    copy(expectedPostOrder.rbegin(), expectedPostOrder.rend(), back_inserter(expectedReversePostOrder));

    QCOMPARE(actualReversePreOrder, expectedReversePreOrder);
    QCOMPARE(actualReverseInOrder, expectedReverseInOrder);
    QCOMPARE(actualReversePostOrder, expectedReversePostOrder);
}

void TreeIterationTest::iteration_data() {

    QTest::addColumn<binary_tree<string>>("t");
    QTest::addColumn<int>("expectedSize");
    QTest::addColumn<list<string>>("expectedPreOrder");
    QTest::addColumn<list<string>>("expectedInOrder");
    QTest::addColumn<list<string>>("expectedPostOrder");

    /******************************************************************************************************************/
    QTest::newRow("Empty String")
        << binary_tree<string>(n(""))
        << 1
        << list<string>{""}
        << list<string>{""}
        << list<string>{""};

    /******************************************************************************************************************/
    QTest::newRow("Root with a left child")
        // clang-format off
        << binary_tree<string>(
            n("1")(
                n("2")
            )
        )
        // clang-format on
        << 2
        << list<string>{"1", "2"}
        << list<string>{"2", "1"}
        << list<string>{"2", "1"};

    /******************************************************************************************************************/
    QTest::newRow("Root with a right child")
        // clang-format off
        << binary_tree<string>(
            n("1")(
                n(),
                n("2")
            )
        )
        // clang-format on
        << 2
        << list<string>{"1", "2"}
        << list<string>{"1", "2"}
        << list<string>{"2", "1"};

    /******************************************************************************************************************/
    QTest::newRow("Small tree")
        // clang-format off
        << binary_tree<string>(
            n("a")(
                n("b")(
                    n("d")(
                        n("h"),
                        n() // This can be omitted but I want to test it
                    ),
                    n("e")
                ),
                n("c")(
                    n("f")(
                        n("j"),
                        n("k")
                    ),
                    n("g")
                )
            )
        )
        // clang-format on
        << 10
        << list<string>{"a", "b", "d", "h", "e", "c", "f", "j", "k", "g"}
        << list<string>{"h", "d", "b", "e", "a", "j", "f", "k", "c", "g"}
        << list<string>{"h", "d", "e", "b", "j", "k", "f", "g", "c", "a"};

    /******************************************************************************************************************/
    QTest::newRow("Big tree")
        // clang-format off
        << binary_tree<string>(
            n("a")(
                n("b")(
                    n("c")(
                        n("d")(
                            n("e")(
                                n("f"),
                                n("g")
                            ),
                            n("h")(
                                n(),
                                n("o")
                            )
                        ),
                        n("i")(
                            n(),
                            n("n")(
                                n(),
                                n("p")
                            )
                        )
                    ),
                    n("j")(
                        n(),
                        n("m")(
                            n(),
                            n("q")(
                                n(),
                                n("t")
                            )
                        )
                    )
                ),
                n("k")(
                    n(),
                    n("l")(
                        n(),
                        n("r")(
                            n(),
                            n("s")(
                                n(),
                                n("u")
                            )
                        )
                    )
                )
            )
        )
        // clang-format on
        << 21
        << list<string>{"a", "b", "c", "d", "e", "f", "g", "h", "o", "i", "n", "p", "j", "m", "q", "t", "k", "l", "r", "s", "u"}
        << list<string>{"f", "e", "g", "d", "h", "o", "c", "i", "n", "p", "b", "j", "m", "q", "t", "a", "k", "l", "r", "s", "u"}
        << list<string>{"f", "g", "e", "o", "h", "d", "p", "n", "i", "c", "t", "q", "m", "j", "b", "u", "s", "r", "l", "k", "a"};

    /******************************************************************************************************************/
    QTest::newRow("All left child")
        // clang-format off
        << binary_tree<string>(
            n("1")(
                n("2")(
                    n("3")(
                        n("4")(
                            n("5")(
                                n("6")(
                                    n("7")(
                                        n("8")(
                                            n("9")(
                                                n("10")(
                                                    n("11")(
                                                        n("12")(
                                                            n("13")(
                                                                n("14")(
                                                                    n("15")
                                                                )
                                                            )
                                                        )
                                                    )
                                                )
                                            )
                                        )
                                    )
                                )
                            )
                        )
                    )
                )
            )
        )
        // clang-format on
        << 15
        << list<string>{"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15"}
        << list<string>{"15", "14", "13", "12", "11", "10", "9", "8", "7", "6", "5", "4", "3", "2", "1"}
        << list<string>{"15", "14", "13", "12", "11", "10", "9", "8", "7", "6", "5", "4", "3", "2", "1"};

    /******************************************************************************************************************/
    QTest::newRow("All right child")
        // clang-format off
        << binary_tree<string>(
            n("1")(
                n(),
                n("2")(
                    n(),
                    n("3")(
                        n(),
                        n("4")(
                            n(),
                            n("5")(
                                n(),
                                n("6")(
                                    n(),
                                    n("7")(
                                        n(),
                                        n("8")(
                                            n(),
                                            n("9")(
                                                n(),
                                                n("10")(
                                                    n(),
                                                    n("11")(
                                                        n(),
                                                        n("12")(
                                                            n(),
                                                            n("13")(
                                                                n(),
                                                                n("14")(
                                                                    n(),
                                                                    n("15")
                                                                )
                                                            )
                                                        )
                                                    )
                                                )
                                            )
                                        )
                                    )
                                )
                            )
                        )
                    )
                )
            )
        )
        // clang-format on
        << 15
        << list<string>{"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15"}
        << list<string>{"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15"}
        << list<string>{"15", "14", "13", "12", "11", "10", "9", "8", "7", "6", "5", "4", "3", "2", "1"};

    /******************************************************************************************************************/
    QTest::newRow("ZigZag")
        // clang-format off
        << binary_tree<string>(
            n("1")(
                n("2")(
                    n(),
                    n("3")(
                        n("4")(
                            n(),
                            n("5")(
                                n("6")(
                                    n(),
                                    n("7")(
                                        n("8")(
                                            n(),
                                            n("9")(
                                                n("10")(
                                                    n(),
                                                    n("11")(
                                                        n("12")(
                                                            n(),
                                                            n("13")(
                                                                n("14")(
                                                                    n(),
                                                                    n("15")
                                                                )
                                                            )
                                                        )
                                                    )
                                                )
                                            )
                                        )
                                    )
                                )
                            )
                        )
                    )
                )
            )
        )
        // clang-format on
        << 15
        << list<string>{"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15"}
        << list<string>{"2", "4", "6", "8", "10", "12", "14", "15", "13", "11", "9", "7", "5", "3", "1"}
        << list<string>{"15", "14", "13", "12", "11", "10", "9", "8", "7", "6", "5", "4", "3", "2", "1"};
}

QTEST_MAIN(TreeIterationTest);
#include "TreeIterationTest.moc"