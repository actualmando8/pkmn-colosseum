    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    mr r28, r3
    mr r29, r5
    lbz r0, 0x21(r28)
    cmplwi r0, 0x0
    bne @L_80011460
    li r3, 0x1258
    li r4, 0x0
    bl fn_8005D8F8
    li r3, 0x1259
    li r4, 0x0
    bl fn_8005D8F8
    li r3, 0x125a
    li r4, 0x0
    bl fn_8005D8F8
    li r3, 0x125b
    li r4, 0x0
    bl fn_8005D8F8
    mr r31, r28
    li r30, 0x0
    @L_800112F0:
    lwz r0, 0x4(r31)
    cmpwi r0, 0x47
    beq @L_8001132C
    bge @L_80011310
    cmpwi r0, 0x45
    beq @L_8001131C
    bge @L_80011324
    b @L_8001133C
    @L_80011310:
    cmpwi r0, 0x49
    bge @L_8001133C
    b @L_80011334
    @L_8001131C:
    li r3, 0x125b
    b @L_80011340
    @L_80011324:
    li r3, 0x125a
    b @L_80011340
    @L_8001132C:
    li r3, 0x1258
    b @L_80011340
    @L_80011334:
    li r3, 0x1259
    b @L_80011340
    @L_8001133C:
    li r3, 0x0
    @L_80011340:
    cmpwi r3, 0x0
    beq @L_80011350
    li r4, 0x1
    bl fn_8005D8F8
    @L_80011350:
    addi r31, r31, 0x8
    addi r30, r30, 0x1
    cmpwi r30, 0x4
    blt @L_800112F0
    li r3, 0xff
    li r4, 0x125a
    bl fn_80102138
    cmpwi r3, -0x1
    stw r3, 0x8(r1)
    bne @L_80011398
    li r3, 0xff
    li r4, 0x125b
    bl fn_80102138
    cmpwi r3, -0x1
    stw r3, 0x8(r1)
    bne @L_80011398
    li r0, 0x0
    stw r0, 0x8(r1)
    @L_80011398:
    mr r7, r29
    mr r9, r28
    addi r5, r1, 0x8
    li r3, 0xff
    li r4, 0x0
    li r6, 0x0
    li r8, 0x1
    crxor 6, 6, 6
    bl fn_801026A4
    cmpwi r3, 0x125a
    beq @L_800113F4
    bge @L_800113D8
    cmpwi r3, 0x1258
    beq @L_800113E4
    bge @L_800113EC
    b @L_80011404
    @L_800113D8:
    cmpwi r3, 0x125c
    bge @L_80011404
    b @L_800113FC
    @L_800113E4:
    li r4, 0x47
    b @L_80011408
    @L_800113EC:
    li r4, 0x48
    b @L_80011408
    @L_800113F4:
    li r4, 0x46
    b @L_80011408
    @L_800113FC:
    li r4, 0x45
    b @L_80011408
    @L_80011404:
    li r4, -0x1
    @L_80011408:
    li r3, -0x1
    lwz r0, 0x4(r28)
    cmpw r4, r0
    bne @L_80011420
    li r3, 0x0
    b @L_80011484
    @L_80011420:
    lwz r0, 0xc(r28)
    cmpw r4, r0
    bne @L_80011434
    li r3, 0x1
    b @L_80011484
    @L_80011434:
    lwz r0, 0x14(r28)
    cmpw r4, r0
    bne @L_80011448
    li r3, 0x2
    b @L_80011484
    @L_80011448:
    lwz r0, 0x1c(r28)
    cmpw r4, r0
    bne @L_80011484
    li r3, 0x3
    b @L_80011484
    b @L_80011484
    @L_80011460:
    mr r5, r4
    mr r7, r29
    mr r9, r28
    li r3, 0x104
    li r4, 0x0
    li r6, 0x0
    li r8, 0x1
    crxor 6, 6, 6
    bl fn_801026A4
    @L_80011484:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
