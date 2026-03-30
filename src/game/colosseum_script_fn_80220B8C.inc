    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r4, 0x0
    stw r0, 0x24(r1)
    stmw r29, 0x14(r1)
    mr r29, r3
    lwz r5, lbl_8047B610(r13)
    lbz r3, 0x1(r5)
    bl fn_801F025C
    rlwinm r4, r29, 0, 16, 31
    mr r31, r3
    cmplwi r4, 0x37
    beq @L_80220BC8
    cmplwi r4, 0x38
    bne @L_80220C38
@L_80220BC8:
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r30, r3, 0, 16, 31
    mr r3, r31
    li r4, 0x0
    li r5, 0xee
    li r6, 0x0
    bl fn_8012640C
    mr. r29, r3
    beq @L_80221058
    li r4, 0x2c
    li r5, 0x4
    li r6, 0x0
    bl fn_801DDD28
    rlwinm. r0, r3, 0, 24, 31
    beq @L_80221058
    mr r3, r29
    li r4, 0x2c
    li r5, 0x4
    bl fn_801DA9E8
    mr r3, r31
    mr r4, r30
    li r5, 0x1
    bl fn_80265598
    b @L_80221058
@L_80220C38:
    cmplwi r4, 0x39
    beq @L_80220C48
    cmplwi r4, 0x3a
    bne @L_80220CB8
@L_80220C48:
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r29, r3, 0, 16, 31
    mr r3, r31
    li r4, 0x0
    li r5, 0xee
    li r6, 0x0
    bl fn_8012640C
    mr. r30, r3
    beq @L_80221058
    li r4, 0x3c
    li r5, 0x4
    li r6, 0x0
    bl fn_801DDD28
    rlwinm. r0, r3, 0, 24, 31
    beq @L_80221058
    mr r3, r30
    li r4, 0x3c
    li r5, 0x4
    bl fn_801DA9E8
    mr r3, r31
    mr r4, r29
    li r5, 0x1
    bl fn_80265598
    b @L_80221058
@L_80220CB8:
    subi r0, r4, 0xf
    cmplwi r0, 0x25
    bgt @L_80221058
    lis r3, jumptable_8039A130@ha
    rlwinm r0, r0, 2, 0, 29
    addi r3, r3, jumptable_8039A130@l
    lwzx r0, r3, r0
    mtctr r0
    bctr
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r29, r3, 0, 16, 31
    mr r3, r31
    li r4, 0x0
    li r5, 0xee
    li r6, 0x0
    bl fn_8012640C
    mr. r30, r3
    beq @L_80221058
    li r4, 0x26
    li r5, 0x4
    li r6, 0x0
    bl fn_801DDD28
    rlwinm. r0, r3, 0, 24, 31
    beq @L_80221058
    mr r3, r30
    li r4, 0x26
    li r5, 0x4
    bl fn_801DA9E8
    mr r3, r31
    mr r4, r29
    li r5, 0x1
    bl fn_80265598
    b @L_80221058
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r29, r3, 0, 16, 31
    mr r3, r31
    li r4, 0x0
    li r5, 0xee
    li r6, 0x0
    bl fn_8012640C
    mr. r30, r3
    beq @L_80221058
    li r4, 0x28
    li r5, 0x4
    li r6, 0x0
    bl fn_801DDD28
    rlwinm. r0, r3, 0, 24, 31
    beq @L_80221058
    mr r3, r30
    li r4, 0x28
    li r5, 0x4
    bl fn_801DA9E8
    mr r3, r31
    mr r4, r29
    li r5, 0x1
    bl fn_80265598
    b @L_80221058
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r29, r3, 0, 16, 31
    mr r3, r31
    li r4, 0x0
    li r5, 0xee
    li r6, 0x0
    bl fn_8012640C
    mr. r30, r3
    beq @L_80221058
    li r4, 0x2a
    li r5, 0x4
    li r6, 0x0
    bl fn_801DDD28
    rlwinm. r0, r3, 0, 24, 31
    beq @L_80221058
    mr r3, r30
    li r4, 0x2a
    li r5, 0x4
    bl fn_801DA9E8
    mr r3, r31
    mr r4, r29
    li r5, 0x1
    bl fn_80265598
    b @L_80221058
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r29, r3, 0, 16, 31
    mr r3, r31
    li r4, 0x0
    li r5, 0xee
    li r6, 0x0
    bl fn_8012640C
    mr. r30, r3
    beq @L_80221058
    li r4, 0x2c
    li r5, 0x4
    li r6, 0x0
    bl fn_801DDD28
    rlwinm. r0, r3, 0, 24, 31
    beq @L_80221058
    mr r3, r30
    li r4, 0x2c
    li r5, 0x4
    bl fn_801DA9E8
    mr r3, r31
    mr r4, r29
    li r5, 0x1
    bl fn_80265598
    b @L_80221058
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r29, r3, 0, 16, 31
    mr r3, r31
    li r4, 0x0
    li r5, 0xee
    li r6, 0x0
    bl fn_8012640C
    mr. r30, r3
    beq @L_80221058
    li r4, 0x27
    li r5, 0x4
    li r6, 0x0
    bl fn_801DDD28
    rlwinm. r0, r3, 0, 24, 31
    beq @L_80221058
    mr r3, r30
    li r4, 0x27
    li r5, 0x4
    bl fn_801DA9E8
    mr r3, r31
    mr r4, r29
    li r5, 0x1
    bl fn_80265598
    b @L_80221058
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r29, r3, 0, 16, 31
    mr r3, r31
    li r4, 0x0
    li r5, 0xee
    li r6, 0x0
    bl fn_8012640C
    mr. r30, r3
    beq @L_80221058
    li r4, 0x29
    li r5, 0x4
    li r6, 0x0
    bl fn_801DDD28
    rlwinm. r0, r3, 0, 24, 31
    beq @L_80221058
    mr r3, r30
    li r4, 0x29
    li r5, 0x4
    bl fn_801DA9E8
    mr r3, r31
    mr r4, r29
    li r5, 0x1
    bl fn_80265598
    b @L_80221058
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r29, r3, 0, 16, 31
    mr r3, r31
    li r4, 0x0
    li r5, 0xee
    li r6, 0x0
    bl fn_8012640C
    mr. r30, r3
    beq @L_80221058
    li r4, 0x2b
    li r5, 0x4
    li r6, 0x0
    bl fn_801DDD28
    rlwinm. r0, r3, 0, 24, 31
    beq @L_80221058
    mr r3, r30
    li r4, 0x2b
    li r5, 0x4
    bl fn_801DA9E8
    mr r3, r31
    mr r4, r29
    li r5, 0x1
    bl fn_80265598
    b @L_80221058
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r29, r3, 0, 16, 31
    mr r3, r31
    li r4, 0x0
    li r5, 0xee
    li r6, 0x0
    bl fn_8012640C
    mr. r30, r3
    beq @L_80221058
    li r4, 0x3c
    li r5, 0x4
    li r6, 0x0
    bl fn_801DDD28
    rlwinm. r0, r3, 0, 24, 31
    beq @L_80221058
    mr r3, r30
    li r4, 0x3c
    li r5, 0x4
    bl fn_801DA9E8
    mr r3, r31
    mr r4, r29
    li r5, 0x1
    bl fn_80265598
@L_80221058:
    lmw r29, 0x14(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
