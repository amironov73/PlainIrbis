// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "magna/tester.h"
#include "magna/irbis.h"

TESTER(field_create_1)
{
    MarcField field;

    field_create (&field);

    CHECK (field_is_empty (&field));

    field_destroy (&field);
}

TESTER(field_add_1)
{
    MarcField field;

    field_create (&field);

    CHECK (field_add (&field, 'a', TEXT_SPAN ("SubA")));
    CHECK (field_add (&field, 'b', TEXT_SPAN ("SubB")));
    CHECK (field.subfields.len == 2);
    CHECK (!field_is_empty (&field));

    field_destroy (&field);
}

TESTER(field_clear_1)
{
    MarcField field;

    field_create (&field);

    CHECK (field_add (&field, 'a', TEXT_SPAN ("SubA")));
    CHECK (field_add (&field, 'b', TEXT_SPAN ("SubB")));
    CHECK (field.subfields.len == 2);
    CHECK (!field_is_empty (&field));
    CHECK (field_clear (&field) == &field);
    CHECK (field.subfields.len == 0);
    CHECK (field_is_empty (&field));

    field_destroy (&field);
}

TESTER(field_clone_1)
{
    MarcField field1, field2;

    field_create (&field1);

    CHECK (field_clone (&field2, &field1));
    CHECK (field1.tag == field2.tag);
    CHECK (buffer_compare (&field1.value, &field2.value) == 0);
    CHECK (field1.subfields.len == field2.subfields.len);

    field_destroy (&field1);
    field_destroy (&field2);
}

TESTER(field_clone_2)
{
    MarcField field1, field2;

    field_create (&field1);

    CHECK (field_set_value (&field1, TEXT_SPAN ("Value")));
    CHECK (field_add (&field1, 'a', TEXT_SPAN ("SubA")));
    CHECK (field_add (&field1, 'b', TEXT_SPAN ("SubB")));
    CHECK (field_clone (&field2, &field1));
    CHECK (field1.tag == field2.tag);
    CHECK (buffer_compare (&field1.value, &field2.value) == 0);
    CHECK (field1.subfields.len == field2.subfields.len);

    field_destroy (&field1);
    field_destroy (&field2);
}

TESTER(field_decode_1)
{
    MarcField field;

    field_create (&field);
    CHECK (!field_decode (&field, span_null()));

    field_destroy (&field);
}

TESTER(field_decode_2)
{
    MarcField field;

    field_create (&field);
    CHECK (!field_decode (&field, TEXT_SPAN ("Wrong")));

    field_destroy (&field);
}

TESTER(field_decode_3)
{
    MarcField field;

    field_create (&field);
    CHECK (field_decode (&field, TEXT_SPAN ("100#Value")));
    CHECK (field.tag == 100);
    CHECK (buffer_compare_text (&field.value, CBTEXT ("Value")) == 0);
    CHECK (field.subfields.len == 0);

    field_destroy (&field);
}

TESTER(field_decode_4)
{
    MarcField field;
    const SubField *subfield;

    field_create (&field);
    CHECK (field_decode (&field, TEXT_SPAN ("100#^aSubA^bSubB")));
    CHECK (field.tag == 100);
    CHECK (buffer_is_empty (&field.value));
    CHECK (field.subfields.len == 2);
    subfield = field_get_subfield_by_index (&field, 0);
    CHECK (subfield != NULL);
    CHECK (subfield->code == 'a');
    CHECK (buffer_compare_text (&subfield->value, CBTEXT ("SubA")) == 0);
    subfield = field_get_subfield_by_index (&field, 1);
    CHECK (subfield != NULL);
    CHECK (subfield->code == 'b');
    CHECK (buffer_compare_text (&subfield->value, CBTEXT ("SubB")) == 0);

    field_destroy (&field);
}

TESTER(field_decode_5)
{
    MarcField field;
    const SubField *subfield;

    field_create (&field);
    CHECK (field_decode (&field, TEXT_SPAN ("100#Value^aSubA^bSubB")));
    CHECK (field.tag == 100);
    CHECK (buffer_compare_text (&field.value, CBTEXT ("Value")) == 0);
    CHECK (field.subfields.len == 2);
    subfield = field_get_subfield_by_index (&field, 0);
    CHECK (subfield != NULL);
    CHECK (subfield->code == 'a');
    CHECK (buffer_compare_text (&subfield->value, CBTEXT ("SubA")) == 0);
    subfield = field_get_subfield_by_index (&field, 1);
    CHECK (subfield != NULL);
    CHECK (subfield->code == 'b');
    CHECK (buffer_compare_text (&subfield->value, CBTEXT ("SubB")) == 0);

    field_destroy (&field);
}

TESTER(field_decode_6)
{
    MarcField field;

    field_create (&field);
    CHECK (field_decode (&field, TEXT_SPAN ("100#")));
    CHECK (field.tag == 100);
    CHECK (buffer_is_empty (&field.value));
    CHECK (field.subfields.len == 0);

    field_destroy (&field);
}

TESTER(field_get_first_subfield_1)
{
    MarcField field;
    SubField *subfield;

    field_create (&field);
    CHECK (field_add (&field, 'A', TEXT_SPAN ("SubA1")));
    CHECK (field_add (&field, 'A', TEXT_SPAN ("SubA2")));
    CHECK (field_add (&field, 'B', TEXT_SPAN ("SubB")));

    subfield = field_get_first_subfield (&field, 'a');
    CHECK (subfield != NULL);
    CHECK (subfield->code == 'a');
    CHECK (buffer_compare_text (&subfield->value, CBTEXT ("SubA1")) == 0);

    subfield = field_get_first_subfield (&field, 'A');
    CHECK (subfield != NULL);
    CHECK (subfield->code == 'a');
    CHECK (buffer_compare_text (&subfield->value, CBTEXT ("SubA1")) == 0);

    subfield = field_get_first_subfield (&field, 'b');
    CHECK (subfield != NULL);
    CHECK (subfield->code == 'b');
    CHECK (buffer_compare_text (&subfield->value, CBTEXT ("SubB")) == 0);

    subfield = field_get_first_subfield (&field, 'c');
    CHECK (subfield == NULL);

    field_destroy (&field);
}

TESTER(field_get_first_subfield_value_1)
{
    MarcField field;
    Span value;

    field_create (&field);
    CHECK (field_add (&field, 'A', TEXT_SPAN ("SubA1")));
    CHECK (field_add (&field, 'A', TEXT_SPAN ("SubA2")));
    CHECK (field_add (&field, 'B', TEXT_SPAN ("SubB")));

    value = field_get_first_subfield_value (&field, 'a');
    CHECK (span_compare (value, TEXT_SPAN ("SubA1")) == 0);

    value = field_get_first_subfield_value (&field, 'A');
    CHECK (span_compare (value, TEXT_SPAN ("SubA1")) == 0);

    value = field_get_first_subfield_value (&field, 'B');
    CHECK (span_compare (value, TEXT_SPAN ("SubB")) == 0);

    value = field_get_first_subfield_value (&field, 'c');
    CHECK (span_is_empty (value));

    field_destroy (&field);
}

TESTER(field_remove_at_1)
{
    MarcField field;
    const SubField *subfield;

    field_create (&field);
    CHECK (field_add (&field, 'a', TEXT_SPAN ("SubA")));
    CHECK (field_add (&field, 'b', TEXT_SPAN ("SubB")));

    CHECK (field_remove_at (&field, 0) == &field);
    CHECK (field.subfields.len == 1);

    subfield = field_get_subfield_by_index (&field, 0);
    CHECK (subfield != NULL);
    CHECK (subfield->code == 'b');

    CHECK (field_remove_at (&field, 0) == &field);
    CHECK (field.subfields.len == 0);

    field_destroy (&field);
}

TESTER(field_remove_at_2)
{
    MarcField field;
    const SubField *subfield;

    field_create (&field);
    CHECK (field_add (&field, 'a', TEXT_SPAN ("SubA")));
    CHECK (field_add (&field, 'b', TEXT_SPAN ("SubB")));

    CHECK (field_remove_at (&field, 1) == &field);
    CHECK (field.subfields.len == 1);

    subfield = field_get_subfield_by_index (&field, 0);
    CHECK (subfield != NULL);
    CHECK (subfield->code == 'a');

    CHECK (field_remove_at (&field, 0) == &field);
    CHECK (field.subfields.len == 0);

    field_destroy (&field);
}

TESTER(field_remove_subfield_1)
{
    MarcField field;

    field_create (&field);

    CHECK (field_remove_subfield (&field, 'c') == &field);
    CHECK (field.subfields.len == 0);

    field_destroy (&field);
}

TESTER(field_remove_subfield_2)
{
    MarcField field;

    field_create (&field);
    CHECK (field_add (&field, 'a', TEXT_SPAN ("SubA1")));
    CHECK (field_add (&field, 'b', TEXT_SPAN ("SubB")));
    CHECK (field_add (&field, 'a', TEXT_SPAN ("SubA2")));

    CHECK (field_remove_subfield (&field, 'c') == &field);
    CHECK (field.subfields.len == 3);

    field_destroy (&field);
}

TESTER(field_remove_subfield_3)
{
    MarcField field;

    field_create (&field);
    CHECK (field_add (&field, 'a', TEXT_SPAN ("SubA1")));
    CHECK (field_add (&field, 'b', TEXT_SPAN ("SubB")));
    CHECK (field_add (&field, 'a', TEXT_SPAN ("SubA2")));

    CHECK (field_remove_subfield (&field, 'B') == &field);
    CHECK (field.subfields.len == 2);

    CHECK (field_remove_subfield (&field, 'A') == &field);
    CHECK (field.subfields.len == 0);

    field_destroy (&field);
}

TESTER(field_remove_subfield_4)
{
    MarcField field;

    field_create (&field);
    CHECK (field_add (&field, 'a', TEXT_SPAN ("SubA1")));
    CHECK (field_add (&field, 'b', TEXT_SPAN ("SubB")));
    CHECK (field_add (&field, 'a', TEXT_SPAN ("SubA2")));

    CHECK (field_remove_subfield (&field, 'A') == &field);
    CHECK (field.subfields.len == 1);

    CHECK (field_remove_subfield (&field, 'B') == &field);
    CHECK (field.subfields.len == 0);

    field_destroy (&field);
}

TESTER(field_set_subfield_1)
{
    MarcField field;

    field_create (&field);

    CHECK (field_set_subfield (&field, 'a', TEXT_SPAN ("SubA1")));
    CHECK (field.subfields.len == 1);
    CHECK (field_set_subfield (&field, 'a', TEXT_SPAN ("SubA2")));
    CHECK (field.subfields.len == 1);
    CHECK (span_compare (field_get_first_subfield_value (&field, 'a'), TEXT_SPAN ("SubA2")) == 0);

    CHECK (field_set_subfield (&field, 'b', TEXT_SPAN ("SubB")));
    CHECK (field.subfields.len == 2);

    CHECK (field_set_subfield (&field, 'a', span_null()));
    CHECK (field.subfields.len == 1);

    CHECK (field_set_subfield (&field, 'c', span_null()));
    CHECK (field.subfields.len == 1);

    CHECK (field_set_subfield (&field, 'b', span_null()));
    CHECK (field.subfields.len == 0);

    field_destroy (&field);
}

TESTER(field_to_string_1)
{
    MarcField field;
    Buffer output = BUFFER_INIT;
    Span expected = TEXT_SPAN ("0#");

    field_create (&field);

    CHECK (field_to_string (&field, &output));
    CHECK (buffer_compare_span (&output, expected) == 0);

    field_destroy (&field);
    buffer_destroy (&output);
}

TESTER(field_to_string_2)
{
    MarcField field;
    Buffer output = BUFFER_INIT;
    Span expected = TEXT_SPAN ("100#");

    field_create (&field);
    field.tag = 100;

    CHECK (field_to_string (&field, &output));
    CHECK (buffer_compare_span (&output, expected) == 0);

    field_destroy (&field);
    buffer_destroy (&output);
}

TESTER(field_to_string_3)
{
    MarcField field;
    Buffer output = BUFFER_INIT;
    Span expected = TEXT_SPAN ("100#Hello");

    field_create (&field);
    field.tag = 100;

    CHECK (field_set_value (&field, TEXT_SPAN ("Hello")));
    CHECK (field_to_string (&field, &output));
    CHECK (buffer_compare_span (&output, expected) == 0);

    field_destroy (&field);
    buffer_destroy (&output);
}

TESTER(field_to_string_4)
{
    MarcField field;
    Buffer output = BUFFER_INIT;
    Span expected = TEXT_SPAN ("100#^aSubA^bSubB");

    field_create (&field);
    field.tag = 100;
    CHECK (field_add (&field, 'a', TEXT_SPAN ("SubA")));
    CHECK (field_add (&field, 'b', TEXT_SPAN ("SubB")));

    CHECK (field_to_string (&field, &output));
    CHECK (buffer_compare_span (&output, expected) == 0);

    field_destroy (&field);
    buffer_destroy (&output);
}

TESTER(field_to_string_5)
{
    MarcField field;
    Buffer output = BUFFER_INIT;
    Span expected = TEXT_SPAN ("100#Value^aSubA^bSubB");

    field_create (&field);
    field.tag = 100;
    CHECK (field_set_value (&field, TEXT_SPAN ("Value")));
    CHECK (field_add (&field, 'a', TEXT_SPAN ("SubA")));
    CHECK (field_add (&field, 'b', TEXT_SPAN ("SubB")));

    CHECK (field_to_string (&field, &output));
    CHECK (buffer_compare_span (&output, expected) == 0);

    field_destroy (&field);
    buffer_destroy (&output);
}

TESTER(field_verify_1)
{
    MarcField field;

    field_create (&field);

    CHECK (!field_verify (&field));

    field_destroy (&field);
}

TESTER(field_verify_2)
{
    MarcField field;

    field_create (&field);
    field.tag = 100;

    CHECK (!field_verify (&field));

    field_destroy (&field);
}

TESTER(field_verify_3)
{
    MarcField field;

    field_create (&field);
    field.tag = 100;
    CHECK (field_set_value (&field, TEXT_SPAN ("Value")));

    CHECK (field_verify (&field));

    field_destroy (&field);
}

TESTER(field_verify_4)
{
    MarcField field;

    field_create (&field);
    field.tag = 100;
    CHECK (field_add (&field, 'a', TEXT_SPAN ("SubA")));
    CHECK (field_add (&field, 'b', TEXT_SPAN ("SubB")));

    CHECK (field_verify (&field));

    field_destroy (&field);
}

TESTER(field_verify_5)
{
    MarcField field;

    field_create (&field);
    field.tag = 100;
    CHECK (field_set_value (&field, TEXT_SPAN ("Value")));
    CHECK (field_add (&field, 'a', TEXT_SPAN ("SubA")));
    CHECK (field_add (&field, 'b', TEXT_SPAN ("SubB")));

    CHECK (field_verify (&field));

    field_destroy (&field);
}

TESTER(field_verify_6)
{
    MarcField field;

    field_create (&field);
    field.tag = 100;
    CHECK (field_set_value (&field, TEXT_SPAN ("Value")));
    CHECK (field_add (&field, 'a', span_null()));
    CHECK (field_add (&field, 'b', TEXT_SPAN ("SubB")));

    CHECK (!field_verify (&field));

    field_destroy (&field);
}

TESTER(field_verify_7)
{
    MarcField field;

    field_create (&field);
    CHECK (field_set_value (&field, TEXT_SPAN ("Value")));
    CHECK (field_add (&field, 'a', TEXT_SPAN ("SubA")));
    CHECK (field_add (&field, 'b', TEXT_SPAN ("SubB")));

    CHECK (!field_verify (&field));

    field_destroy (&field);
}


