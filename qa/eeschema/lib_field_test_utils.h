/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2019 KiCad Developers, see CHANGELOG.TXT for contributors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, you may find one here:
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
 * or you may search the http://www.gnu.org website for the version 2 license,
 * or you may write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */

/**
 * @file
 * Test utils (e.g. print helpers and test predicates for LIB_FIELD objects
 */

#ifndef QA_EESCHEMA_LIB_FIELD_TEST_UTILS__H
#define QA_EESCHEMA_LIB_FIELD_TEST_UTILS__H

#include <unit_test_utils/unit_test_utils.h>

#include <class_libentry.h>
#include <template_fieldnames.h>


namespace BOOST_TEST_PRINT_NAMESPACE_OPEN
{
template <>
struct print_log_value<LIB_FIELD>
{
    inline void operator()( std::ostream& os, LIB_FIELD const& f )
    {
        os << "LIB_FIELD[ " << f.GetName() << " ]";
    }
};

template <>
struct print_log_value<LIB_FIELDS>
{
    inline void operator()( std::ostream& os, LIB_FIELDS const& f )
    {
        os << "LIB_FIELDS[ " << f.size() << " ]";
    }
};
}
BOOST_TEST_PRINT_NAMESPACE_CLOSE


namespace KI_TEST
{

/**
 * Predicate to check a field name is as expected
 * @param  aField    LIB_FIELD to check the name
 * @param  aExpectedName the expected field name
 * @param  aExpectedId the expected field id
 * @return           true if match
 */
bool FieldNameIdMatches(
        const LIB_FIELD& aField, const std::string& aExpectedName, int aExpectedId )
{
    bool       ok = true;
    const auto gotName = aField.GetName( false );

    if( gotName != aExpectedName )
    {
        BOOST_TEST_INFO(
                "Field name mismatch: got '" << gotName << "', expected '" << aExpectedName );
        ok = false;
    }

    const int gotId = aField.GetId();

    if( gotId != aExpectedId )
    {
        BOOST_TEST_INFO( "Field ID mismatch: got '" << gotId << "', expected '" << aExpectedId );
        ok = false;
    }

    return ok;
}

/**
 * Predicate to check that the mandatory fields in a LIB_FIELDS object look sensible
 * @param  aFields the fields to check
 * @return         true if valid
 */
bool AreDefaultFieldsCorrect( const LIB_FIELDS& aFields )
{
    const unsigned expectedCount = NumFieldType::MANDATORY_FIELDS;
    if( aFields.size() < expectedCount )
    {
        BOOST_TEST_INFO(
                "Expected at least " << expectedCount << " fields, got " << aFields.size() );
        return false;
    }

    bool ok = true;

    ok &= FieldNameIdMatches( aFields[0], "Reference", NumFieldType::REFERENCE );
    ok &= FieldNameIdMatches( aFields[1], "Value", NumFieldType::VALUE );
    ok &= FieldNameIdMatches( aFields[2], "Footprint", NumFieldType::FOOTPRINT );
    ok &= FieldNameIdMatches( aFields[3], "Datasheet", NumFieldType::DATASHEET );

    return ok;
}

} // namespace KI_TEST

#endif // QA_EESCHEMA_LIB_FIELD_TEST_UTILS__H