// Copyright 2015 Juan Luis Álvarez Martínez
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
// http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <stdint.h>

/* --- Datasets as extracted from a real Z80 --- */
#include "dataset-add.h"
#include "dataset-adc.h"
#include "dataset-sub.h"
#include "dataset-sbc.h"
#include "dataset-cp.h"
#include "dataset-and.h"
#include "dataset-or.h"
#include "dataset-xor.h"

#include "dataset-inc.h"
#include "dataset-dec.h"

#include "dataset-neg.h"

#include "test_z80.h"
#include "z80/z80_opcodes.h"

TEST_GROUP(alu8_internal);

TEST_SETUP(alu8_internal) {
}

TEST_TEAR_DOWN(alu8_internal) {
}

TEST(alu8_internal, neg) {
    //Exhaust de ALU8 NEG test space
    for (int a = 0; a < 256; a++) {
        uint16_t expected_af = z80_dataset_neg[a + (256 * 0)];
        uint8_t expected_a = (expected_af >> 8) & 0x00FF;
        uint8_t expected_f = (expected_af) & 0x00FF;

        z80.opcode_index = 2;
        Z80_A = (uint8_t)a;
        Z80_F = 0xFF;
        NEG();

        char test_str[100];
        sprintf(test_str, "In A:%02X; Out AF:%02X%02X; Expected:%04X.", a, Z80_A, Z80_F, expected_af);
        TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_a, Z80_A, test_str);
        TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_f, Z80_F, test_str);
    }

    //Same, with F=0xFF initially.
    for (int a = 0; a < 256; a++) {
        uint16_t expected_af = z80_dataset_neg[a + (256 * 1)];
        uint8_t expected_a = (expected_af >> 8) & 0x00FF;
        uint8_t expected_f = (expected_af) & 0x00FF;

        z80.opcode_index = 2;
        Z80_A = (uint8_t)a;
        Z80_F = 0xFF;
        NEG();

        char test_str[100];
        sprintf(test_str, "In A:%02X; Out AF:%02X%02X; Expected:%04X.", a, Z80_A, Z80_F, expected_af);
        TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_a, Z80_A, test_str);
        TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_f, Z80_F, test_str);
    }
}

TEST(alu8_internal, inc) {
    alu_result_t r;
    //Exhaust de ALU8 INC test space
    for (int a = 0; a < 256; a++) {
        uint16_t expected_af = z80_dataset_inc[a + (256 * 0)];
        uint8_t expected_a = (expected_af >> 8) & 0x00FF;
        uint8_t expected_f = (expected_af) & 0x00FF;
        r = alu8_inc(a, 0x00);

        char test_str[100];
        sprintf(test_str, "In A:%02X; Out AF:%02X%02X; Expected:%04X.", a, (uint8_t)r.result, r.flags, expected_af);
        TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_a, r.result, test_str);
        TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_f, r.flags, test_str);
    }

    //Same, with F=0xFF initially.
    for (int a = 0; a < 256; a++) {
        uint16_t expected_af = z80_dataset_inc[a + (256 * 1)];
        uint8_t expected_a = (expected_af >> 8) & 0x00FF;
        uint8_t expected_f = (expected_af) & 0x00FF;
        r = alu8_inc(a, 0xFF);

        char test_str[100];
        sprintf(test_str, "In A:%02X; Out AF:%02X%02X; Expected:%04X.", a, (uint8_t)r.result, r.flags, expected_af);
        TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_a, r.result, test_str);
        TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_f, r.flags, test_str);
    }
}

TEST(alu8_internal, dec) {
    alu_result_t r;
    //Exhaust de ALU8 DEC test space
    for (int a = 0; a < 256; a++) {
        uint16_t expected_af = z80_dataset_dec[a + (256 * 0)];
        uint8_t expected_a = (expected_af >> 8) & 0x00FF;
        uint8_t expected_f = (expected_af) & 0x00FF;
        r = alu8_dec(a, 0x00);

        char test_str[100];
        sprintf(test_str, "In A:%02X; Out AF:%02X%02X; Expected:%04X.", a, (uint8_t)r.result, r.flags, expected_af);
        TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_a, r.result, test_str);
        TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_f, r.flags, test_str);
    }

    //Same, with F=0xFF initially.
    for (int a = 0; a < 256; a++) {
        uint16_t expected_af = z80_dataset_dec[a + (256 * 1)];
        uint8_t expected_a = (expected_af >> 8) & 0x00FF;
        uint8_t expected_f = (expected_af) & 0x00FF;
        r = alu8_dec(a, 0xFF);

        char test_str[100];
        sprintf(test_str, "In A:%02X; Out AF:%02X%02X; Expected:%04X.", a, (uint8_t)r.result, r.flags, expected_af);
        TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_a, r.result, test_str);
        TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_f, r.flags, test_str);
    }
}

TEST(alu8_internal, add) {
    alu_result_t r;
    //Exhaust the ALU8 ADD test space, F=0x00
    for (int a = 0; a < 256; a++) {
        for (int b = 0; b < 256; b++) {
            uint16_t expected_af = z80_dataset_add[(a * 256) + b];
            uint8_t expected_a = (expected_af >> 8) & 0x00FF;
            uint8_t expected_f = (expected_af) & 0x00FF;
            r = alu8_op(Z80_ALUOP_ADD, a, b, 0x00);

            char test_str[100];
            sprintf(test_str, "In A:%02X B:%02X; Out AF:%02X%02X; Expected:%04X.", a, b, (uint8_t)r.result, r.flags, expected_af);
            TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_a, r.result, test_str);
            TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_f, r.flags, test_str);

        }
    }

    //Exhaust the ALU8 ADD test space, F=0xFF
    for (int a = 0; a < 256; a++) {
        for (int b = 0; b < 256; b++) {
            uint16_t expected_af = z80_dataset_add[(a * 256) + b + (256 * 256)];
            uint8_t expected_a = (expected_af >> 8) & 0x00FF;
            uint8_t expected_f = (expected_af) & 0x00FF;
            r = alu8_op(Z80_ALUOP_ADD, a, b, 0xFF);

            char test_str[100];
            sprintf(test_str, "In A:%02X B:%02X; Out AF:%02X%02X; Expected:%04X.", a, b, (uint8_t)r.result, r.flags, expected_af);
            TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_a, r.result, test_str);
            TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_f, r.flags, test_str);
        }
    }
}

TEST(alu8_internal, and) {
    alu_result_t r;
    //Exhaust the ALU8 AND test space, F=0x00
    for (int a = 0; a < 256; a++) {
        for (int b = 0; b < 256; b++) {
            uint16_t expected_af = z80_dataset_and[(a * 256) + b];
            uint8_t expected_a = (expected_af >> 8) & 0x00FF;
            uint8_t expected_f = (expected_af) & 0x00FF;
            r = alu8_op(Z80_ALUOP_AND, a, b, 0x00);

            char test_str[100];
            sprintf(test_str, "In A:%02X B:%02X; Out AF:%02X%02X; Expected:%04X.", a, b, (uint8_t)r.result, r.flags, expected_af);
            TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_a, r.result, test_str);
            TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_f, r.flags, test_str);

        }
    }

    //Exhaust the ALU8 AND test space, F=0xFF
    for (int a = 0; a < 256; a++) {
        for (int b = 0; b < 256; b++) {
            uint16_t expected_af = z80_dataset_and[(a * 256) + b + (256 * 256)];
            uint8_t expected_a = (expected_af >> 8) & 0x00FF;
            uint8_t expected_f = (expected_af) & 0x00FF;
            r = alu8_op(Z80_ALUOP_AND, a, b, 0xFF);

            char test_str[100];
            sprintf(test_str, "In A:%02X B:%02X; Out AF:%02X%02X; Expected:%04X.", a, b, (uint8_t)r.result, r.flags, expected_af);
            TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_a, r.result, test_str);
            TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_f, r.flags, test_str);
        }
    }
}

TEST(alu8_internal, or ) {
    alu_result_t r;
    //Exhaust the ALU8 OR test space, F=0x00
    for (int a = 0; a < 256; a++) {
        for (int b = 0; b < 256; b++) {
            uint16_t expected_af = z80_dataset_or[(a * 256) + b];
            uint8_t expected_a = (expected_af >> 8) & 0x00FF;
            uint8_t expected_f = (expected_af) & 0x00FF;
            r = alu8_op(Z80_ALUOP_OR, a, b, 0x00);

            char test_str[100];
            sprintf(test_str, "In A:%02X B:%02X; Out AF:%02X%02X; Expected:%04X.", a, b, (uint8_t)r.result, r.flags, expected_af);
            TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_a, r.result, test_str);
            TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_f, r.flags, test_str);

        }
    }

    //Exhaust the ALU8 OR test space, F=0xFF
    for (int a = 0; a < 256; a++) {
        for (int b = 0; b < 256; b++) {
            uint16_t expected_af = z80_dataset_or[(a * 256) + b + (256 * 256)];
            uint8_t expected_a = (expected_af >> 8) & 0x00FF;
            uint8_t expected_f = (expected_af) & 0x00FF;
            r = alu8_op(Z80_ALUOP_OR, a, b, 0xFF);

            char test_str[100];
            sprintf(test_str, "In A:%02X B:%02X; Out AF:%02X%02X; Expected:%04X.", a, b, (uint8_t)r.result, r.flags, expected_af);
            TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_a, r.result, test_str);
            TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_f, r.flags, test_str);
        }
    }
}

TEST(alu8_internal, sub) {
    alu_result_t r;
    //Exhaust the ALU8 SUB test space, F=0x00
    for (int a = 0; a < 256; a++) {
        for (int b = 0; b < 256; b++) {
            uint16_t expected_af = z80_dataset_sub[(a * 256) + b];
            uint8_t expected_a = (expected_af >> 8) & 0x00FF;
            uint8_t expected_f = (expected_af) & 0x00FF;
            r = alu8_op(Z80_ALUOP_SUB, a, b, 0x00);

            char test_str[100];
            sprintf(test_str, "In A:%02X B:%02X; Out AF:%02X%02X; Expected:%04X.", a, b, (uint8_t)r.result, r.flags, expected_af);
            TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_a, r.result, test_str);
            TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_f, r.flags, test_str);

        }
    }

    //Exhaust the ALU8 SUB test space, F=0xFF
    for (int a = 0; a < 256; a++) {
        for (int b = 0; b < 256; b++) {
            uint16_t expected_af = z80_dataset_sub[(a * 256) + b + (256 * 256)];
            uint8_t expected_a = (expected_af >> 8) & 0x00FF;
            uint8_t expected_f = (expected_af) & 0x00FF;
            r = alu8_op(Z80_ALUOP_SUB, a, b, 0xFF);

            char test_str[100];
            sprintf(test_str, "In A:%02X B:%02X; Out AF:%02X%02X; Expected:%04X.", a, b, (uint8_t)r.result, r.flags, expected_af);
            TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_a, r.result, test_str);
            TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_f, r.flags, test_str);
        }
    }
}

TEST(alu8_internal, xor) {
    alu_result_t r;
    //Exhaust the ALU8 XOR test space, F=0x00
    for (int a = 0; a < 256; a++) {
        for (int b = 0; b < 256; b++) {
            uint16_t expected_af = z80_dataset_xor[(a * 256) + b];
            uint8_t expected_a = (expected_af >> 8) & 0x00FF;
            uint8_t expected_f = (expected_af) & 0x00FF;
            r = alu8_op(Z80_ALUOP_XOR, a, b, 0x00);

            char test_str[100];
            sprintf(test_str, "In A:%02X B:%02X; Out AF:%02X%02X; Expected:%04X.", a, b, (uint8_t)r.result, r.flags, expected_af);
            TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_a, r.result, test_str);
            TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_f, r.flags, test_str);

        }
    }

    //Exhaust the ALU8 XOR test space, F=0xFF
    for (int a = 0; a < 256; a++) {
        for (int b = 0; b < 256; b++) {
            uint16_t expected_af = z80_dataset_xor[(a * 256) + b + (256 * 256)];
            uint8_t expected_a = (expected_af >> 8) & 0x00FF;
            uint8_t expected_f = (expected_af) & 0x00FF;
            r = alu8_op(Z80_ALUOP_XOR, a, b, 0xFF);

            char test_str[100];
            sprintf(test_str, "In A:%02X B:%02X; Out AF:%02X%02X; Expected:%04X.", a, b, (uint8_t)r.result, r.flags, expected_af);
            TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_a, r.result, test_str);
            TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_f, r.flags, test_str);
        }
    }
}

TEST(alu8_internal, cp) {
    alu_result_t r;
    //Exhaust the ALU8 XOR test space, F=0x00
    for (int a = 0; a < 256; a++) {
        for (int b = 0; b < 256; b++) {
            uint16_t expected_af = z80_dataset_cp[(a * 256) + b];
            uint8_t expected_a = (expected_af >> 8) & 0x00FF;
            uint8_t expected_f = (expected_af) & 0x00FF;
            r = alu8_op(Z80_ALUOP_CP, a, b, 0x00);

            char test_str[100];
            sprintf(test_str, "In A:%02X B:%02X; Out AF:%02X%02X; Expected:%04X.", a, b, (uint8_t)r.result, r.flags, expected_af);
            TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_a, r.result, test_str);
            TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_f, r.flags, test_str);

        }
    }

    //Exhaust the ALU8 XOR test space, F=0xFF
    for (int a = 0; a < 256; a++) {
        for (int b = 0; b < 256; b++) {
            uint16_t expected_af = z80_dataset_cp[(a * 256) + b + (256 * 256)];
            uint8_t expected_a = (expected_af >> 8) & 0x00FF;
            uint8_t expected_f = (expected_af) & 0x00FF;
            r = alu8_op(Z80_ALUOP_CP, a, b, 0xFF);

            char test_str[100];
            sprintf(test_str, "In A:%02X B:%02X; Out AF:%02X%02X; Expected:%04X.", a, b, (uint8_t)r.result, r.flags, expected_af);
            TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_a, r.result, test_str);
            TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_f, r.flags, test_str);
        }
    }
}

TEST(alu8_internal, adc) {
	alu_result_t r;
	//Exhaust the ALU8 ADC test space, F=0x00
	for (int a = 0; a < 256; a++) {
		for (int b = 0; b < 256; b++) {
			uint16_t expected_af = z80_dataset_adc[(a * 256) + b + (256 * 256 * 0)];
			uint8_t expected_a = (expected_af >> 8) & 0x00FF;
			uint8_t expected_f = (expected_af) & 0x00FF;
			r = alu8_op(Z80_ALUOP_ADC, a, b, 0x00);

			char test_str[100];
			sprintf(test_str, "In A:%02X B:%02X; Out AF:%02X%02X; Expected:%04X.", a, b, (uint8_t)r.result, r.flags, expected_af);
			TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_a, r.result, test_str);
			TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_f, r.flags, test_str);

		}
	}

    //Exhaust the ALU8 ADC test space, F=0xFF - Carry
    for (int a = 0; a < 256; a++) {
        for (int b = 0; b < 256; b++) {
            uint16_t expected_af = z80_dataset_adc[(a * 256) + b + (256 * 256 * 3)];
            uint8_t expected_a = (expected_af >> 8) & 0x00FF;
            uint8_t expected_f = (expected_af) & 0x00FF;
            r = alu8_op(Z80_ALUOP_ADC, a, b, 0xFF & Z80_CLRFLAG_CARRY);

            char test_str[100];
            sprintf(test_str, "In A:%02X B:%02X; Out AF:%02X%02X; Expected:%04X.", a, b, (uint8_t)r.result, r.flags, expected_af);
            TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_a, r.result, test_str);
            TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_f, r.flags, test_str);
        }
    }

	//Exhaust the ALU8 ADC test space, F=0x00 + Carry
	for (int a = 0; a < 256; a++) {
		for (int b = 0; b < 256; b++) {
			uint16_t expected_af = z80_dataset_adc[(a * 256) + b + (256 * 256 * 1)];
			uint8_t expected_a = (expected_af >> 8) & 0x00FF;
			uint8_t expected_f = (expected_af) & 0x00FF;
			r = alu8_op(Z80_ALUOP_ADC, a, b, 0x00 | Z80_FLAG_CARRY);

			char test_str[100];
			sprintf(test_str, "In A:%02X B:%02X; Out AF:%02X%02X; Expected:%04X.", a, b, (uint8_t)r.result, r.flags, expected_af);
			TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_a, r.result, test_str);
			TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_f, r.flags, test_str);
		}
	}

	//Exhaust the ALU8 ADC test space, F=0xFF
	for (int a = 0; a < 256; a++) {
		for (int b = 0; b < 256; b++) {
			uint16_t expected_af = z80_dataset_adc[(a * 256) + b + (256 * 256 * 2)];
			uint8_t expected_a = (expected_af >> 8) & 0x00FF;
			uint8_t expected_f = (expected_af) & 0x00FF;
			r = alu8_op(Z80_ALUOP_ADC, a, b, 0xFF);

			char test_str[100];
			sprintf(test_str, "In A:%02X B:%02X; Out AF:%02X%02X; Expected:%04X.", a, b, (uint8_t)r.result, r.flags, expected_af);
			TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_a, r.result, test_str);
			TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_f, r.flags, test_str);
		}
	}
}

TEST(alu8_internal, sbc) {
	alu_result_t r;
	//Exhaust the ALU8 SBC test space, F=0x00
	for (int a = 0; a < 256; a++) {
		for (int b = 0; b < 256; b++) {
			uint16_t expected_af = z80_dataset_sbc[(a * 256) + b + (256 * 256 * 0)];
			uint8_t expected_a = (expected_af >> 8) & 0x00FF;
			uint8_t expected_f = (expected_af) & 0x00FF;
			r = alu8_op(Z80_ALUOP_SBC, a, b, 0x00);

			char test_str[100];
			sprintf(test_str, "In A:%02X B:%02X; Out AF:%02X%02X; Expected:%04X.", a, b, (uint8_t)r.result, r.flags, expected_af);
			TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_a, r.result, test_str);
			TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_f, r.flags, test_str);

		}
	}

    //Exhaust the ALU8 SBC test space, F=0xFF - Carry
    for (int a = 0; a < 256; a++) {
        for (int b = 0; b < 256; b++) {
            uint16_t expected_af = z80_dataset_sbc[(a * 256) + b + (256 * 256 * 3)];
            uint8_t expected_a = (expected_af >> 8) & 0x00FF;
            uint8_t expected_f = (expected_af) & 0x00FF;
            r = alu8_op(Z80_ALUOP_SBC, a, b, 0xFF & Z80_CLRFLAG_CARRY);

            char test_str[100];
            sprintf(test_str, "In A:%02X B:%02X; Out AF:%02X%02X; Expected:%04X.", a, b, (uint8_t)r.result, r.flags, expected_af);
            TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_a, r.result, test_str);
            TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_f, r.flags, test_str);
        }
    }

	//Exhaust the ALU8 SBC test space, F=0x00 + Carry
	for (int a = 0; a < 256; a++) {
		for (int b = 0; b < 256; b++) {
			uint16_t expected_af = z80_dataset_sbc[(a * 256) + b + (256 * 256 * 1)];
			uint8_t expected_a = (expected_af >> 8) & 0x00FF;
			uint8_t expected_f = (expected_af) & 0x00FF;
			r = alu8_op(Z80_ALUOP_SBC, a, b, 0x00 | Z80_FLAG_CARRY);

			char test_str[100];
			sprintf(test_str, "In A:%02X B:%02X; Out AF:%02X%02X; Expected:%04X.", a, b, (uint8_t)r.result, r.flags, expected_af);
			TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_a, r.result, test_str);
			TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_f, r.flags, test_str);
		}
	}

	//Exhaust the ALU8 SBC test space, F=0xFF
	for (int a = 0; a < 256; a++) {
		for (int b = 0; b < 256; b++) {
			uint16_t expected_af = z80_dataset_sbc[(a * 256) + b + (256 * 256 * 2)];
			uint8_t expected_a = (expected_af >> 8) & 0x00FF;
			uint8_t expected_f = (expected_af) & 0x00FF;
			r = alu8_op(Z80_ALUOP_SBC, a, b, 0xFF);

			char test_str[100];
			sprintf(test_str, "In A:%02X B:%02X; Out AF:%02X%02X; Expected:%04X.", a, b, (uint8_t)r.result, r.flags, expected_af);
			TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_a, r.result, test_str);
			TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_f, r.flags, test_str);
		}
	}
}

TEST_GROUP_RUNNER(alu8_internal) {
    RUN_TEST_CASE(alu8_internal, neg);
    RUN_TEST_CASE(alu8_internal, inc);
    RUN_TEST_CASE(alu8_internal, dec);
    RUN_TEST_CASE(alu8_internal, add);
    RUN_TEST_CASE(alu8_internal, and);
    RUN_TEST_CASE(alu8_internal, or );
    RUN_TEST_CASE(alu8_internal, sub);
    RUN_TEST_CASE(alu8_internal, xor);
    RUN_TEST_CASE(alu8_internal, adc);
    RUN_TEST_CASE(alu8_internal, sbc);
    RUN_TEST_CASE(alu8_internal, cp );
}

// ----------------------
// --- Helpers & Main ---
// ----------------------
//Helper 'run all' function
static void RunAllTests(void) {
    RUN_TEST_GROUP(alu8_internal);
}

//Main
int main(int argc, const char** argv) {
    int rv = UnityMain(argc, argv, RunAllTests);
    return rv;
}
