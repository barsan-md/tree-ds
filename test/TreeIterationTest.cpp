#include <QtTest/QtTest>
#include <algorithm>
#include <iterator>
#include <vector>

#include <TreeDS/tree>

using namespace std;
using namespace md;

Q_DECLARE_METATYPE(vector<char>);
Q_DECLARE_METATYPE(binary_tree<char>);

class TreeIterationTest : public QObject {

    Q_OBJECT

    private slots:
    void iteration();
    void iteration_data();
};

void TreeIterationTest::iteration() {
    QFETCH(binary_tree<char>, binary);
    QFETCH(int, expectedSize);
    QFETCH(int, expectedArity);
    QFETCH(vector<char>, expectedPreOrder);
    QFETCH(vector<char>, expectedInOrder);
    QFETCH(vector<char>, expectedPostOrder);
    QFETCH(vector<char>, expectedBreadthFirst);
    QFETCH(vector<char>, expectedLeaves);
    nary_tree<char> nary(binary);
    int actualSize  = static_cast<int>(binary.size());
    int actualArity = static_cast<int>(binary.arity());

    // Preliminary test
    QCOMPARE(nary, binary);
    QCOMPARE(actualSize, expectedSize);
    QCOMPARE(actualArity, expectedArity);

    /*   ---   Forward order test   ---   */
    // Binary
    vector<char> actualBinaryPreOrder(binary.begin(policy::pre_order()), binary.end(policy::pre_order()));
    vector<char> actualBinaryInOrder(binary.begin(policy::in_order()), binary.end(policy::in_order()));
    vector<char> actualBinaryPostOrder(binary.begin(policy::post_order()), binary.end(policy::post_order()));
    vector<char> actualBinaryBreadthFirst(binary.begin(policy::breadth_first()), binary.end(policy::breadth_first()));
    vector<char> actualBinaryLeaves(binary.begin(policy::leaves()), binary.end(policy::leaves()));

    // Compare results binary
    QCOMPARE(actualBinaryPreOrder, expectedPreOrder);
    QCOMPARE(actualBinaryInOrder, expectedInOrder);
    QCOMPARE(actualBinaryPostOrder, expectedPostOrder);
    QCOMPARE(actualBinaryBreadthFirst, expectedBreadthFirst);
    QCOMPARE(actualBinaryLeaves, expectedLeaves);

    // Nary
    vector<char> actualNaryPreOrder(nary.begin(policy::pre_order()), nary.end(policy::pre_order()));
    vector<char> actualNaryPostOrder(nary.begin(policy::post_order()), nary.end(policy::post_order()));
    vector<char> actualNaryBreadthFirst(nary.begin(policy::breadth_first()), nary.end(policy::breadth_first()));
    vector<char> actualNaryLeaves(nary.begin(policy::leaves()), nary.end(policy::leaves()));

    // Compare results nary
    QCOMPARE(actualNaryPreOrder, expectedPreOrder);
    QCOMPARE(actualNaryPostOrder, expectedPostOrder);
    QCOMPARE(actualNaryBreadthFirst, expectedBreadthFirst);
    QCOMPARE(actualNaryLeaves, expectedLeaves);

    /*   ---   Backward order test   ---   */
    vector<char> expectedReversePreOrder(expectedPreOrder.rbegin(), expectedPreOrder.rend());
    vector<char> expectedReverseInOrder(expectedInOrder.rbegin(), expectedInOrder.rend());
    vector<char> expectedReversePostOrder(expectedPostOrder.rbegin(), expectedPostOrder.rend());
    vector<char> expectedReverseBreadthFirst(expectedBreadthFirst.rbegin(), expectedBreadthFirst.rend());
    vector<char> expectedReverseLeaves(expectedLeaves.rbegin(), expectedLeaves.rend());

    // Binary
    vector<char> actualBinaryReversePreOrder(binary.rbegin(policy::pre_order()), binary.rend(policy::pre_order()));
    vector<char> actualBinaryReverseInOrder(binary.rbegin(policy::in_order()), binary.rend(policy::in_order()));
    vector<char> actualBinaryReversePostOrder(binary.rbegin(policy::post_order()), binary.rend(policy::post_order()));
    vector<char> actualBinaryReverseBreadthFirst(binary.rbegin(policy::breadth_first()), binary.rend(policy::breadth_first()));
    vector<char> actualBinaryReverseLeaves(binary.rbegin(policy::leaves()), binary.rend(policy::leaves()));

    // Compare results binary
    QCOMPARE(actualBinaryReversePreOrder, expectedReversePreOrder);
    QCOMPARE(actualBinaryReverseInOrder, expectedReverseInOrder);
    QCOMPARE(actualBinaryReversePostOrder, expectedReversePostOrder);
    QCOMPARE(actualBinaryReverseBreadthFirst, expectedReverseBreadthFirst);
    QCOMPARE(actualBinaryReverseLeaves, expectedReverseLeaves);

    // Nary
    vector<char> actualNaryReversePreOrder(nary.rbegin(policy::pre_order()), nary.rend(policy::pre_order()));
    vector<char> actualNaryReversePostOrder(nary.rbegin(policy::post_order()), nary.rend(policy::post_order()));
    vector<char> actualNaryReverseBreadthFirst(nary.rbegin(policy::breadth_first()), nary.rend(policy::breadth_first()));
    vector<char> actualNaryReverseLeaves(nary.rbegin(policy::leaves()), nary.rend(policy::leaves()));

    // Compare results nary
    QCOMPARE(actualNaryReversePreOrder, expectedReversePreOrder);
    QCOMPARE(actualNaryReversePostOrder, expectedReversePostOrder);
    QCOMPARE(actualNaryReverseBreadthFirst, expectedReverseBreadthFirst);
    QCOMPARE(actualNaryReverseLeaves, expectedReverseLeaves);
}

void TreeIterationTest::iteration_data() {
    QTest::addColumn<binary_tree<char>>("binary");
    QTest::addColumn<int>("expectedSize");
    QTest::addColumn<int>("expectedArity");
    QTest::addColumn<vector<char>>("expectedPreOrder");
    QTest::addColumn<vector<char>>("expectedInOrder");
    QTest::addColumn<vector<char>>("expectedPostOrder");
    QTest::addColumn<vector<char>>("expectedBreadthFirst");
    QTest::addColumn<vector<char>>("expectedLeaves");

    /******************************************************************************************************************/
    QTest::newRow("Single character")
        << binary_tree<char>(n('#'))
        << 1
        << 0
        << vector {'#'}
        << vector {'#'}
        << vector {'#'}
        << vector {'#'}
        << vector {'#'};

    /******************************************************************************************************************/
    QTest::newRow("Root with a left child")
        << binary_tree<char>(
               n('1')(
                   n('2')))
        << 2
        << 1
        << vector {'1', '2'}
        << vector {'2', '1'}
        << vector {'2', '1'}
        << vector {'1', '2'}
        << vector {'2'};

    /******************************************************************************************************************/
    QTest::newRow("Root with a right child")
        << binary_tree<char>(
               n('1')(
                   n(),
                   n('2')))
        << 2
        << 1
        << vector {'1', '2'}
        << vector {'1', '2'}
        << vector {'2', '1'}
        << vector {'1', '2'}
        << vector {'2'};

    /******************************************************************************************************************/
    QTest::newRow("Small tree")
        << binary_tree<char>(
               n('a')(
                   n('b')(
                       n('d')(
                           n('h'),
                           n() // This can be omitted but I want to test it
                           ),
                       n('e')),
                   n('c')(
                       n('f')(
                           n('j'),
                           n('k')),
                       n('g'))))
        << 10
        << 2
        << vector {'a', 'b', 'd', 'h', 'e', 'c', 'f', 'j', 'k', 'g'}
        << vector {'h', 'd', 'b', 'e', 'a', 'j', 'f', 'k', 'c', 'g'}
        << vector {'h', 'd', 'e', 'b', 'j', 'k', 'f', 'g', 'c', 'a'}
        << vector {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'j', 'k'}
        << vector {'h', 'e', 'j', 'k', 'g'};

    /******************************************************************************************************************/
    QTest::newRow("Big tree")
        << binary_tree<char>(
               n('a')(
                   n('b')(
                       n('c')(
                           n('d')(
                               n('e')(
                                   n('f'),
                                   n('g')),
                               n('h')(
                                   n(),
                                   n('o'))),
                           n('i')(
                               n(),
                               n('n')(
                                   n(),
                                   n('p')))),
                       n('j')(
                           n(),
                           n('m')(
                               n(),
                               n('q')(
                                   n(),
                                   n('t'))))),
                   n('k')(
                       n(),
                       n('l')(
                           n(),
                           n('r')(
                               n(),
                               n('s')(
                                   n(),
                                   n('u')))))))
        << 21
        << 2
        << vector {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'o', 'i', 'n', 'p', 'j', 'm', 'q', 't', 'k', 'l', 'r', 's', 'u'}
        << vector {'f', 'e', 'g', 'd', 'h', 'o', 'c', 'i', 'n', 'p', 'b', 'j', 'm', 'q', 't', 'a', 'k', 'l', 'r', 's', 'u'}
        << vector {'f', 'g', 'e', 'o', 'h', 'd', 'p', 'n', 'i', 'c', 't', 'q', 'm', 'j', 'b', 'u', 's', 'r', 'l', 'k', 'a'}
        << vector {'a', 'b', 'k', 'c', 'j', 'l', 'd', 'i', 'm', 'r', 'e', 'h', 'n', 'q', 's', 'f', 'g', 'o', 'p', 't', 'u'}
        << vector {'f', 'g', 'o', 'p', 't', 'u'};

    /******************************************************************************************************************/
    QTest::newRow("All left child")
        << binary_tree<char>(
               n('1')(
                   n('2')(
                       n('3')(
                           n('4')(
                               n('5')(
                                   n('6')(
                                       n('7')(
                                           n('8')(
                                               n('9')(
                                                   n('A')(
                                                       n('B')(
                                                           n('C')(
                                                               n('D')(
                                                                   n('E')(
                                                                       n('F'))))))))))))))))
        << 15
        << 1
        << vector {'1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'}
        << vector {'F', 'E', 'D', 'C', 'B', 'A', '9', '8', '7', '6', '5', '4', '3', '2', '1'}
        << vector {'F', 'E', 'D', 'C', 'B', 'A', '9', '8', '7', '6', '5', '4', '3', '2', '1'}
        << vector {'1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'}
        << vector {'F'};

    /******************************************************************************************************************/
    QTest::newRow("All right child")
        << binary_tree<char>(
               n('1')(
                   n(),
                   n('2')(
                       n(),
                       n('3')(
                           n(),
                           n('4')(
                               n(),
                               n('5')(
                                   n(),
                                   n('6')(
                                       n(),
                                       n('7')(
                                           n(),
                                           n('8')(
                                               n(),
                                               n('9')(
                                                   n(),
                                                   n('A')(
                                                       n(),
                                                       n('B')(
                                                           n(),
                                                           n('C')(
                                                               n(),
                                                               n('D')(
                                                                   n(),
                                                                   n('E')(
                                                                       n(),
                                                                       n('F'))))))))))))))))
        << 15
        << 1
        << vector {'1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'}
        << vector {'1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'}
        << vector {'F', 'E', 'D', 'C', 'B', 'A', '9', '8', '7', '6', '5', '4', '3', '2', '1'}
        << vector {'1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'}
        << vector {'F'};

    /******************************************************************************************************************/
    QTest::newRow("ZigZag")
        << binary_tree<char>(
               n('1')(
                   n('2')(
                       n(),
                       n('3')(
                           n('4')(
                               n(),
                               n('5')(
                                   n('6')(
                                       n(),
                                       n('7')(
                                           n('8')(
                                               n(),
                                               n('9')(
                                                   n('A')(
                                                       n(),
                                                       n('B')(
                                                           n('C')(
                                                               n(),
                                                               n('D')(
                                                                   n('E')(
                                                                       n(),
                                                                       n('F'))))))))))))))))
        << 15
        << 1
        << vector {'1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'}
        << vector {'2', '4', '6', '8', 'A', 'C', 'E', 'F', 'D', 'B', '9', '7', '5', '3', '1'}
        << vector {'F', 'E', 'D', 'C', 'B', 'A', '9', '8', '7', '6', '5', '4', '3', '2', '1'}
        << vector {'1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'}
        << vector {'F'};
}

QTEST_MAIN(TreeIterationTest);
#include "TreeIterationTest.moc"
