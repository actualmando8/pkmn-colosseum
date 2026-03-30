    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    lis r7, -32698
    stw r0, 0x24(r1)
    stmw r26, 0x8(r1)
    mr r28, r5
    mr r27, r4
    addi r31, r7, 28904
    mr r26, r3
    mr r29, r6
    mr r4, r28
    addi r3, r31, 0x40
    li r5, 0x14
    bl memcpy
    lwz r0, 0x0(r28)
    addi r3, r31, 0x68
    add r30, r26, r29
    nor r0, r0, r0
    stw r0, 0x0(r3)
    lwz r0, 0x4(r28)
    nor r0, r0, r0
    stw r0, 0x4(r3)
    lwz r0, 0x8(r28)
    nor r0, r0, r0
    stw r0, 0x8(r3)
    lwz r0, 0xc(r28)
    nor r0, r0, r0
    stw r0, 0xc(r3)
    lwz r0, 0x10(r28)
    nor r0, r0, r0
    stw r0, 0x10(r3)
    b @L_801CBDD8
@L_801CBD5C:
    mr r4, r30
    addi r3, r31, 0x54
    li r5, 0x14
    bl fn_801CBBAC
    addi r7, r31, 0x68
    lwz r6, 0x0(r30)
    lwz r0, 0x0(r7)
    mr r3, r7
    addi r4, r31, 0x54
    li r5, 0x14
    xor r0, r6, r0
    stw r0, 0x0(r30)
    lwz r6, 0x4(r30)
    lwz r0, 0x4(r7)
    xor r0, r6, r0
    stw r0, 0x4(r30)
    lwz r6, 0x8(r30)
    lwz r0, 0x8(r7)
    xor r0, r6, r0
    stw r0, 0x8(r30)
    lwz r6, 0xc(r30)
    lwz r0, 0xc(r7)
    xor r0, r6, r0
    stw r0, 0xc(r30)
    lwz r0, 0x10(r7)
    lwz r6, 0x10(r30)
    xor r0, r6, r0
    stw r0, 0x10(r30)
    bl memcpy
    addi r30, r30, 0x14
    addi r29, r29, 0x14
@L_801CBDD8:
    cmplw r29, r27
    blt @L_801CBD5C
    mr r4, r26
    mr r5, r27
    addi r3, r31, 0x68
    bl fn_801CBBAC
    addi r6, r31, 0x40
    addi r5, r31, 0x68
    li r4, 0x5
    b @L_801CBE20
@L_801CBE00:
    lwz r3, 0x0(r5)
    addi r5, r5, 0x4
    lwz r0, 0x0(r6)
    addi r6, r6, 0x4
    cmplw r3, r0
    beq @L_801CBE20
    li r3, 0x0
    b @L_801CBE30
@L_801CBE20:
    cmpwi r4, 0x0
    subi r4, r4, 0x1
    bgt @L_801CBE00
    li r3, 0x1
@L_801CBE30:
    lmw r26, 0x8(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
