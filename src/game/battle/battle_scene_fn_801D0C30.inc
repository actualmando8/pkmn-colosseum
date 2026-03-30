    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    li r31, 0x1
    stw r30, 0x18(r1)
    li r30, 0x0
    stw r29, 0x14(r1)
    li r29, 0x0
    stw r28, 0x10(r1)
@L_801D0C58:
    cmpwi r30, 0x9
    beq @L_801D0D48
    bge @L_801D0C88
    cmpwi r30, 0x6
    beq @L_801D0CFC
    bge @L_801D0C7C
    cmpwi r30, 0x0
    beq @L_801D0CAC
    b @L_801D0D88
@L_801D0C7C:
    cmpwi r30, 0x8
    bge @L_801D0D2C
    b @L_801D0D88
@L_801D0C88:
    cmpwi r30, 0xc
    beq @L_801D0CDC
    bge @L_801D0CA0
    cmpwi r30, 0xb
    bge @L_801D0D64
    b @L_801D0CC0
@L_801D0CA0:
    cmpwi r30, 0x3e8
    beq @L_801D0D84
    b @L_801D0D88
@L_801D0CAC:
    li r29, 0x0
    li r3, 0x1
    bl fn_801D0AFC
    li r30, 0xa
    b @L_801D0D88
@L_801D0CC0:
    lfs f1, -21788(r2)
    li r3, 0x3
    bl fn_801C41C8
    li r30, 0xc
    bl fn_8016557C
    mr r28, r3
    b @L_801D0D88
@L_801D0CDC:
    li r3, 0x0
    bl fn_801C40F0
    extsb. r0, r3
    bne @L_801D0CF4
    li r30, 0x6
    b @L_801D0D88
@L_801D0CF4:
    bl fn_800F0308
    b @L_801D0D88
@L_801D0CFC:
    addi r29, r29, 0x1
    cmpwi r29, 0xa
    blt @L_801D0D24
    li r30, 0x8
    li r3, 0x19
    li r4, 0x7d0
    li r5, 0xff
    bl fn_801653CC
    li r29, 0x0
    b @L_801D0D88
@L_801D0D24:
    bl fn_800F0308
    b @L_801D0D88
@L_801D0D2C:
    addi r29, r29, 0x1
    cmpwi r29, 0xa
    blt @L_801D0D40
    li r30, 0x9
    b @L_801D0D88
@L_801D0D40:
    bl fn_800F0308
    b @L_801D0D88
@L_801D0D48:
    mr r3, r28
    bl fn_80165548
    lfs f1, -21788(r2)
    li r3, 0x2
    bl fn_801C41C8
    li r30, 0xb
    b @L_801D0D88
@L_801D0D64:
    li r3, 0x0
    bl fn_801C40F0
    extsb. r0, r3
    bne @L_801D0D7C
    li r30, 0x3e8
    b @L_801D0D88
@L_801D0D7C:
    bl fn_800F0308
    b @L_801D0D88
@L_801D0D84:
    li r31, 0x0
@L_801D0D88:
    cmpwi r31, 0x0
    bne @L_801D0C58
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
