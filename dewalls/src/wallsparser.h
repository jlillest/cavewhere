#ifndef DEWALLS_WALLSPARSER_H
#define DEWALLS_WALLSPARSER_H

#include <QString>
#include <QHash>
#include <QSharedPointer>
#include <QStack>
#
#include "lineparser.h"
#include "unitizeddouble.h"
#include "length.h"
#include "angle.h"
#include "cardinaldirection.h"
#include "varianceoverride.h"
#include "wallstypes.h"
#include "wallsunits.h"
#include "wallsvisitor.h"

namespace dewalls {

class WallsParser : public LineParser
{
public:
    typedef UnitizedDouble<Length> ULength;
    typedef UnitizedDouble<Angle>  UAngle;
    typedef const Unit<Length> * LengthUnit;
    typedef const Unit<Angle>  * AngleUnit;
    typedef QHash<QChar, LengthUnit> LengthUnitSuffixMap;
    typedef QHash<QChar, AngleUnit>  AngleUnitSuffixMap;
    typedef QHash<QChar, CardinalDirection::CardinalDirection> CardinalDirectionCharMap;
    typedef QSharedPointer<VarianceOverride> VarianceOverridePtr;
    typedef void (WallsParser::*OwnProduction)();

    WallsParser();
    WallsParser(Segment segment);

    WallsVisitor* visitor() const;
    void setVisitor(WallsVisitor* visitor);

    QSharedPointer<WallsUnits> units() const;

    ULength unsignedLengthInches();
    ULength unsignedLengthNonInches(LengthUnit defaultUnit);
    ULength unsignedLength(LengthUnit defaultUnit);
    ULength length(LengthUnit defaultUnit);

    UAngle unsignedAngle(AngleUnitSuffixMap unitSuffixes, AngleUnit defaultUnit);
    UAngle unsignedDmsAngle();

    UAngle latitude();
    UAngle longitude();

    UAngle nonQuadrantAzimuth(AngleUnit defaultUnit);
    UAngle quadrantAzimuth(AngleUnit defaultUnit);
    UAngle azimuth(AngleUnit defaultUnit);
    UAngle azimuthOffset(AngleUnit defaultUnit);

    UAngle unsignedInclination(AngleUnit defaultUnit);
    UAngle inclination(AngleUnit defaultUnit);

    VarianceOverridePtr varianceOverride(LengthUnit defaultUnit);
    VarianceOverridePtr floatedVectorVarianceOverride();
    VarianceOverridePtr floatedTraverseVarianceOverride();
    VarianceOverridePtr lengthVarianceOverride(LengthUnit defaultUnit);
    VarianceOverridePtr rmsErrorVarianceOverride(LengthUnit defaultUnit);

    template<typename F>
    QChar escapedChar(F charPredicate, std::initializer_list<QString> expectedItems);
    template<typename F>
    QString escapedText(F charPredicate, std::initializer_list<QString> expectedItems);
    QString quotedTextOrNonwhitespace();
    QString quotedText();

    template<typename R, typename F>
    bool optional(R& result, F production);

    template<typename T>
    QList<T> elementChars(QHash<QChar, T> elements, QSet<T> requiredElements);

    void replaceMacros();
    QString movePastEndQuote();
    QString replaceMacro();

    void beginBlockCommentLine();
    void endBlockCommentLine();
    void insideBlockCommentLine();
    QString untilComment(std::initializer_list<QString> expectedItems);

    void segmentLine();
    QString segmentDirective();

    void prefixLine();
    void prefixDirective();

    void noteLine();
    void noteDirective();

    void flagLine();
    void flagDirective();
    QString slashPrefixedFlag();

    void symbolLine();

    void dateLine();
    void dateDirective();
    QDate isoDate();
    QDate usDate1();
    QDate usDate2();

    void unitsLine();
    void unitsOptions();
    void unitsOption();
    void macroOption();
    void save();
    void restore();
    void reset();
    void meters();
    void feet();
    void ct();
    void d();
    void s();
    void a();
    void ab();
    void a_ab();
    void v();
    void vb();
    void v_vb();
    void order();
    void ctOrder();
    void rectOrder();
    void decl();
    void grid();
    void rect();
    void incd();
    void inch();
    void incs();
    void inca();
    void incab();
    void incv();
    void incvb();
    void typeab();
    void typevb();
    void case_();
    void lrud();
    void lrudOrder();
    void prefix1();
    void prefix2();
    void prefix3();
    void prefix(int index);
    void tape();
    void uvh();
    void uvv();
    void uv();
    void flag();

    void inlineCommentOrEndOfLine();
    void inlineComment();

private:
    static double approx(double val);

    static const QList<QPair<QString, LengthUnit>> lengthUnits;
    static const QList<QPair<QString, AngleUnit>> azmUnits;
    static const QList<QPair<QString, AngleUnit>> incUnits;
    static const LengthUnitSuffixMap lengthUnitSuffixes;
    static const AngleUnitSuffixMap azmUnitSuffixes;
    static const AngleUnitSuffixMap incUnitSuffixes;
    static const CardinalDirectionCharMap cardinalDirections;
    static const CardinalDirectionCharMap northSouth;
    static const CardinalDirectionCharMap eastWest;
    static const QHash<QChar, QChar> escapedChars;
    static const QHash<QChar, CtElement> ctElements;
    static const QSet<CtElement> requiredCtElements;
    static const QHash<QChar, RectElement> rectElements;
    static const QSet<RectElement> requiredRectElements;
    static const QHash<QChar, LrudElement> lrudElements;
    static const QSet<LrudElement> requiredLrudElements;
    static const QList<QPair<QString, bool>> correctedValues;
    static const QList<QPair<QString, CaseType>> caseTypes;
    static const QList<QPair<QString, LrudType>> lrudTypes;
    static const QList<QPair<QString, QList<TapingMethodElement>>> tapingMethods;
    static const QList<QPair<QString, int>> prefixDirectives;

    static const QRegExp notSemicolon;
    static const QRegExp unitsOptionRx;
    static const QRegExp macroNameRx;
    static const QRegExp stationRx;
    static const QRegExp prefixRx;

    static const QRegExp optionalRx;
    static const QRegExp optionalStationRx;

    static const QRegExp isoDateRx;
    static const QRegExp usDateRx1;
    static const QRegExp usDateRx2;

    static const QHash<QString, OwnProduction> unitsOptionMap;

    WallsVisitor* _visitor;
    bool _inBlockComment;
    QSharedPointer<WallsUnits> _units;
    QStack<QSharedPointer<WallsUnits>> _stack;
    QHash<QString, QString> _macros;
};

inline WallsVisitor* WallsParser::visitor() const
{
    return _visitor;
}

inline void WallsParser::setVisitor(WallsVisitor* visitor)
{
    _visitor = visitor;
}

inline QSharedPointer<WallsUnits> WallsParser::units() const
{
    return _units;
}

template<typename F>
QChar WallsParser::escapedChar(F charPredicate, std::initializer_list<QString> expectedItems)
{
    QChar c = expectChar(charPredicate, expectedItems);
    return c == '\\' ? oneOfMap(escapedChars) : c;
}

template<typename F>
QString WallsParser::escapedText(F charPredicate, std::initializer_list<QString> expectedItems)
{
    QString result;
    while (maybe([&]() { result.append(this->escapedChar(charPredicate, expectedItems)); } ));
    return result;
}

template<typename R, typename F>
bool WallsParser::optional(R& result, F production)
{
    if (maybe([&]() { return this->expect(optionalPattern); }))
    {
        return false;
    }
    result = production();
    return true;
}

template<typename T>
QList<T> WallsParser::elementChars(QHash<QChar, T> elements, QSet<T> requiredElements)
{
    QList<T> result;
    while (!elements.isEmpty())
    {
        T element;
        if (requiredElements.isEmpty()) {
            if (!maybe([&]() { this->oneOfMap(elements); }))
            {
                break;
            }
        }
        else {
            element = oneOfMap(elements);
        }
        result += element;
        QChar c = _line.at(_i - 1);
        elements.remove(c.toLower());
        elements.remove(c.toUpper());
        requiredElements -= element;
    }
    return result;
}

} // namespace dewalls

#endif // DEWALLS_WALLSPARSER_H
