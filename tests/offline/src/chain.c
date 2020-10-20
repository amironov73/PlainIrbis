// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "magna/tester.h"

TESTER(chain_init_1)
{
    ChainSpan chain = CHAIN_INIT;

    CHECK (chain.start == NULL);
    CHECK (chain.end   == NULL);
    CHECK (chain.next  == NULL);
    CHECK (chain_is_empty (&chain));

    chain_destroy (&chain);
    CHECK (chain.start == NULL);
    CHECK (chain.end   == NULL);
    CHECK (chain.next  == NULL);
}

TESTER(chain_init_2)
{
    Span span = TEXT_SPAN ("Hello, world!");
    ChainSpan chain;

    chain_init (&chain, span);
    CHECK (chain.start == span.start);
    CHECK (chain.end   == span.end);
    CHECK (chain.next  == NULL);
    CHECK (!chain_is_empty (&chain));

    chain_destroy (&chain);
    CHECK (chain.start == NULL);
    CHECK (chain.end   == NULL);
    CHECK (chain.next  == NULL);
}

TESTER(chain_null_1)
{
    ChainSpan chain = chain_null();

    CHECK (chain.start == NULL);
    CHECK (chain.end   == NULL);
    CHECK (chain.next  == NULL);
    CHECK (chain_is_empty (&chain));

    chain_destroy (&chain);
    CHECK (chain.start == NULL);
    CHECK (chain.end   == NULL);
    CHECK (chain.next  == NULL);
}

TESTER(chain_last_1)
{
    ChainSpan chain = CHAIN_INIT, *last;

    last = chain_last (&chain);
    CHECK (last == &chain);

    chain_destroy (&chain);
}

TESTER(chain_last_2)
{
    Span span = TEXT_SPAN ("Hello, world!");
    ChainSpan chain, *last;

    chain_init (&chain, span);
    last = chain_last (&chain);
    CHECK (last == &chain);

    chain_destroy (&chain);
}

TESTER(chain_append_1)
{
    Span span1 = TEXT_SPAN ("Hello");
    Span span2 = TEXT_SPAN ("World");
    ChainSpan chain;

    chain_init (&chain, span1);
    CHECK (chain_append (&chain, span2));
    CHECK (chain.next != NULL);
    CHECK (chain_last (&chain) == chain.next);
    CHECK (chain.next->start == span2.start);
    CHECK (chain.next->end   == span2.end);
    CHECK (chain.next->next  == NULL);

    chain_destroy (&chain);
    CHECK (chain.start == NULL);
    CHECK (chain.end   == NULL);
    CHECK (chain.next  == NULL);
}

TESTER(chain_append_2)
{
    Span span1 = TEXT_SPAN ("Hello");
    Span span2 = TEXT_SPAN ("World");
    Span span3 = TEXT_SPAN ("Again");
    ChainSpan chain;

    chain_init (&chain, span1);
    CHECK (chain_append (&chain, span2));
    CHECK (chain_append (&chain, span3));
    CHECK (chain.next != NULL);
    CHECK (chain.next->next != NULL);
    CHECK (chain_last (&chain) == chain.next->next);
    CHECK (chain.next->next->start == span3.start);
    CHECK (chain.next->next->end   == span3.end);
    CHECK (chain.next->next->next  == NULL);

    chain_destroy (&chain);
    CHECK (chain.start == NULL);
    CHECK (chain.end   == NULL);
    CHECK (chain.next  == NULL);
}

TESTER(chain_total_length_1)
{
    ChainSpan chain = CHAIN_INIT;

    CHECK (chain_total_length (&chain) == 0u);

    chain_destroy (&chain);
}

TESTER(chain_total_length_2)
{
    Span span = TEXT_SPAN ("Hello, world!");
    ChainSpan chain;

    chain_init (&chain, span);
    CHECK (chain_total_length (&chain) == span_length (span));

    chain_destroy (&chain);
}

TESTER(chain_total_length_3)
{
    Span span1 = TEXT_SPAN ("Hello");
    Span span2 = TEXT_SPAN ("World");
    ChainSpan chain;

    chain_init (&chain, span1);
    CHECK (chain_append (&chain, span2));
    CHECK (chain_total_length (&chain) == span_length (span1) + span_length (span2));

    chain_destroy (&chain);
}

TESTER(chain_total_length_4)
{
    Span span1 = TEXT_SPAN ("Hello");
    Span span2 = TEXT_SPAN ("World");
    Span span3 = TEXT_SPAN ("Again");
    ChainSpan chain;

    chain_init (&chain, span1);
    CHECK (chain_append (&chain, span2));
    CHECK (chain_append (&chain, span3));
    CHECK (chain_total_length (&chain)
        == span_length (span1) + span_length (span2) + span_length (span3));

    chain_destroy (&chain);
}

TESTER(chain_is_empty_1)
{
    ChainSpan chain = CHAIN_INIT;

    CHECK (chain_append (&chain, span_null()));
    CHECK (chain_is_empty (&chain));
    CHECK (chain_append (&chain, span_null()));
    CHECK (chain_is_empty (&chain));

    chain_destroy (&chain);
}

TESTER(chain_from_text_1)
{
    const am_byte *text = CBTEXT ("Hello, world!");
    ChainSpan chain = chain_from_text (text);

    CHECK (chain.start == text);
    CHECK (chain.end   == text + strlen (CCTEXT (text)));
    CHECK (chain.next  == NULL);

    chain_destroy (&chain);
}

TESTER(chain_before_1)
{
    ChainSpan chain = CHAIN_INIT;

    CHECK (chain_before (&chain, &chain) == NULL);

    chain_destroy (&chain);
}

TESTER(chain_before_2)
{
    Span span1 = TEXT_SPAN ("Hello");
    Span span2 = TEXT_SPAN ("World");
    ChainSpan chain;

    chain_init (&chain, span1);
    CHECK (chain_append (&chain, span2));

    CHECK (chain_before (&chain, &chain) == NULL);
    CHECK (chain_before (&chain, chain.next) == &chain);

    chain_destroy (&chain);
}

TESTER(chain_before_3)
{
    Span span1 = TEXT_SPAN ("Hello");
    Span span2 = TEXT_SPAN ("World");
    Span span3 = TEXT_SPAN ("Again");
    ChainSpan chain;

    chain_init (&chain, span1);
    CHECK (chain_append (&chain, span2));
    CHECK (chain_append (&chain, span3));

    CHECK (chain_before (&chain, &chain) == NULL);
    CHECK (chain_before (&chain, chain.next) == &chain);
    CHECK (chain_before (&chain, chain.next->next) == chain.next);

    chain_destroy (&chain);
}

TESTER(chain_before_4)
{
    ChainSpan chain1 = CHAIN_INIT, chain2 = CHAIN_INIT;

    CHECK (chain_before (&chain1, &chain2) == NULL);

    chain_destroy (&chain1);
    chain_destroy (&chain2);
}
