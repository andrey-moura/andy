#include <andy/xml.hpp>

#include <andy/tests.hpp>

using namespace andy::tests;

describe of(structure, "xml", ([]() {
  describe(static_method, "decode", []() {
    auto tag_expectation = [](const andy::xml& subject, std::string_view tag) {
      it("should have the {} tag", {tag}, [&]() {
        expect(its tag).to<eq>(tag);
      });
    };
    auto empty_expectations = [&](const andy::xml& subject, std::string_view tag = "root", bool testing_declaration_or_model = false) {
      tag_expectation(subject, tag);

      it("should have no content", [&]() {
        expect(its content).to<be_empty>();
      });
      
      it("should have no childrens", [&]() {
        expect(its childrens).to<be_empty>();
      });

      if(!testing_declaration_or_model) {
        it("should have no attributes", [&]() {
          expect(its attributes).to<be_empty>();
        });
      }
    }; 
    auto empty_describer = [=](std::string_view xml) {
      context(std::format("decoding '{}'", xml), [=]() {
        empty_expectations(andy::xml::decode(xml));
      });
    };

    empty_describer("<root/>");
    empty_describer("<root></root>");
    empty_describer("\t<root>      \n     </root>    \r        ");

    context("decoding xml with declaration", [=]() {
      subject(andy::xml::decode("<?xml version=\"1.0\" encoding=\"UTF-8\"?><root></root>"));

      empty_expectations(subject);

      it("should have declaration", [&]() {
        expect(its declaration).to_not<eq>(nullptr);
      });

      describe("its declaration", [=]() {
        empty_expectations(*subject.declaration, "xml", true);
        
        it("should have 2 attributes", [&]() {
          expect(its declaration->attributes.size()).to<eq>(2);
        });

        it("should have version 1.0", [&]() {
          expect(its declaration->try_attribute("version")).to<eq>("1.0");
        });

        it("should have encoding UTF-8", [&]() {
          expect(its declaration->try_attribute("encoding")).to<eq>("UTF-8");
        });
      });
    });
    context("decoding xml with model", [=]() {
      subject(andy::xml::decode("<?xml-model href=\"schema.rng\" type=\"application/xml\"?><root></root>"));
      empty_expectations(subject);
    });
    context("decoding xml with childreans", [=]() {
      subject(andy::xml::decode("<root><child1/><child2/></root>"));

      tag_expectation(subject, "root");

      it("should have 2 childreans", [&]() {
        expect(its childrens.size()).to<eq>(2);
      });

      describe("its first childrean", [=]() {
        empty_expectations(subject.childrens[0], "child1");
      });

      describe("its second childrean", [=]() {
        empty_expectations(subject.childrens[1], "child2");
      });
    });
    context("deconding xml with content", [=]() {
      subject(andy::xml::decode("<root>content</root>"));

      tag_expectation(subject, "root");

      it("should have content", [&]() {
        expect(its content).to<eq>("content");
      });
    });
    context("decoding xml tag with ':' character", [=]() {
      subject(andy::xml::decode("<root:tag></root:tag>"));
      tag_expectation(subject, "root:tag");
    });
    context("decoding xml tag with '_' character", [=]() {
      subject(andy::xml::decode("<root_tag></root_tag>"));
      tag_expectation(subject, "root_tag");
    });
    context("decoding xml tag with '-' character", [=]() {
      subject(andy::xml::decode("<root-tag></root-tag>"));
      tag_expectation(subject, "root-tag");
    });
    context("decoding xml tag with number character", [=]() {
      subject(andy::xml::decode("<root1></root1>"));
      tag_expectation(subject, "root1");
    });
    context("decoding xml tag with number character", [=]() {
      subject(andy::xml::decode("<root1></root1>"));
      tag_expectation(subject, "root1");
    });
    context("decoding xml with comments", [=]() {
      subject(andy::xml::decode("<!-- comment --><root></root>"));
      empty_expectations(subject);
    });
    context("decoding xml with comments and childreans", [=]() {
      subject(andy::xml::decode("<root><!-- comment --><child1/><!-- comment --><child2/></root>"));

      tag_expectation(subject, "root");

      it("should have 2 childreans", [&]() {
        expect(its childrens.size()).to<eq>(2);
      });

      describe("its first childrean", [=]() {
        empty_expectations(subject.childrens[0], "child1");
      });

      describe("its second childrean", [=]() {
        empty_expectations(subject.childrens[1], "child2");
      });
    });
  });
}));