    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r27, 0x1c(r1)
    mr r27, r3
    mr r28, r4
    mr r29, r5
    mr r30, r6
    bl fn_800F92D4
    mr r31, r3
    cmplwi r31, 0x0
    bne @L_801C9E0C
    lis r3, -32729
    mr r4, r27
    addi r3, r3, 22700
    crxor 6, 6, 6
    bl fn_800DD970
    b @L_801C9E68
@L_801C9E0C:
    mr r4, r28
    bl fn_800DCB78
    lfs f1, -21912(r2)
    mr r3, r31
    bl fn_800DCAF0
    xoris r3, r29, 0x8000
    lis r0, 0x4330
    stw r3, 0xc(r1)
    mr r3, r31
    lfd f1, -21888(r2)
    stw r0, 0x8(r1)
    lfd f0, 0x8(r1)
    fsubs f1, f0, f1
    bl fn_800DCADC
    cmpwi r30, 0x0
    beq @L_801C9E5C
    mr r3, r31
    li r4, 0x1
    bl fn_800DCAD4
    b @L_801C9E68
@L_801C9E5C:
    mr r3, r31
    li r4, 0x0
    bl fn_800DCAD4
@L_801C9E68:
    lmw r27, 0x1c(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
