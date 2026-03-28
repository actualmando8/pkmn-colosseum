    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x24(r1)
    stmw r29, 0x14(r1)
    mr r31, r4
    mr r29, r3
    bne @L_801ED0FC
    li r3, 0x0
    li r4, 0xb
    bl fn_80129280
    mr r29, r3
@L_801ED0FC:
    cmplwi r31, 0x0
    bne @L_801ED10C
    li r3, 0x0
    b @L_801ED204
@L_801ED10C:
    mr r3, r31
    bl fn_8011F4A8
    cmplwi r29, 0x0
    mr r30, r3
    mr r3, r29
    bne @L_801ED130
    li r3, 0x0
    li r4, 0xb
    bl fn_80129280
@L_801ED130:
    cmplwi r3, 0x0
    beq @L_801ED13C
    stb r30, 0x1(r3)
@L_801ED13C:
    cmplwi r29, 0x0
    mr r3, r29
    bne @L_801ED154
    li r3, 0x0
    li r4, 0xb
    bl fn_80129280
@L_801ED154:
    addic. r30, r3, 0x8
    bne @L_801ED164
    li r3, 0x0
    b @L_801ED204
@L_801ED164:
    mr r3, r31
    li r4, 0x3e
    bl fn_80121ADC
    clrlwi. r0, r3, 24
    beq @L_801ED184
    mr r3, r31
    li r4, 0x3e
    bl fn_80121B4C
@L_801ED184:
    li r0, 0x27
    subi r5, r30, 0x4
    subi r4, r31, 0x4
    mtctr r0
@L_801ED194:
    lwz r3, 0x4(r4)
    lwzu r0, 0x8(r4)
    stw r3, 0x4(r5)
    stwu r0, 0x8(r5)
    bdnz @L_801ED194
    mr r3, r31
    bl fn_8011ED68
    clrlwi. r0, r3, 24
    beq @L_801ED1C8
    mr r3, r31
    bl fn_8011EE28
    stw r3, 0x4(r29)
    b @L_801ED1D0
@L_801ED1C8:
    li r0, 0x0
    stw r0, 0x4(r29)
@L_801ED1D0:
    cmplwi r29, 0x0
    bne @L_801ED1E8
    li r3, 0x0
    li r4, 0xb
    bl fn_80129280
    mr r29, r3
@L_801ED1E8:
    cmplwi r29, 0x0
    beq @L_801ED1F8
    li r0, 0x1
    stb r0, 0x0(r29)
@L_801ED1F8:
    mr r3, r31
    bl fn_80124A60
    li r3, 0x1
@L_801ED204:
    lmw r29, 0x14(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
