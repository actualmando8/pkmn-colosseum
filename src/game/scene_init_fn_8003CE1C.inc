    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    lwz r0, lbl_8047A4D0(r13)
    cmplwi r0, 0x0
    beq @L_8003CF18
    lwz r4, lbl_8047A4D4(r13)
    slwi r0, r3, 2
    lis r3, lbl_803A6748@ha
    add r4, r4, r0
    addi r3, r3, lbl_803A6748@l
    lhz r31, 0x2(r4)
    addi r30, r3, 0x94
    mr r3, r31
    mr r4, r30
    bl fn_801EE544
    lbz r0, 0x0(r30)
    cmpwi r0, 0x2
    beq @L_8003CE8C
    bge @L_8003CF10
    cmpwi r0, 0x0
    bge @L_8003CE84
    b @L_8003CF10
    @L_8003CE84:
    li r3, 0x3720
    b @L_8003CF10
    @L_8003CE8C:
    mr r3, r31
    li r4, 0x0
    bl fn_801EEFAC
    mr r30, r3
    clrlwi r0, r30, 16
    cmplwi r0, 0x9
    bne @L_8003CEBC
    mr r3, r31
    bl fn_801EE174
    clrlwi r3, r3, 24
    bl fn_801FBD58
    b @L_8003CEFC
    @L_8003CEBC:
    mr r3, r31
    li r4, 0x1
    bl fn_801EEFAC
    mr r29, r3
    clrlwi r0, r29, 16
    cmplwi r0, 0x0
    beq @L_8003CEEC
    mr r3, r31
    bl fn_801EEF40
    cmplwi r3, 0x3
    beq @L_8003CEEC
    mr r30, r29
    @L_8003CEEC:
    clrlwi r3, r30, 16
    bl fn_801FCCC4
    bl fn_801FCC64
    bl fn_801FBD58
    @L_8003CEFC:
    cmplwi r31, 0x43
    bne @L_8003CF0C
    li r3, 0x12c0
    b @L_8003CF10
    @L_8003CF0C:
    bl fn_801FBD28
    @L_8003CF10:
    bl fn_800FA280
    b @L_8003CF1C
    @L_8003CF18:
    li r3, 0x0
    @L_8003CF1C:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
