    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr. r31, r3
    bne @L_801F8020
    li r3, 0x0
    b @L_801F80EC
    @L_801F8020:
    bne @L_801F802C
    li r0, 0x0
    b @L_801F80A4
    @L_801F802C:
    bl fn_801EF634
    clrlwi r0, r3, 16
    cmplwi r0, 0x1
    bne @L_801F8044
    li r0, 0x0
    b @L_801F80A4
    @L_801F8044:
    mr r3, r31
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    cmpwi r3, 0x0
    bne @L_801F8068
    li r0, 0x0
    b @L_801F80A4
    @L_801F8068:
    mr r3, r31
    li r4, 0x0
    li r5, 0x44
    li r6, 0x0
    bl fn_801FB1C0
    cmplwi r3, 0x0
    bne @L_801F808C
    li r0, 0x0
    b @L_801F80A4
    @L_801F808C:
    bl fn_8012A130
    clrlwi. r0, r3, 24
    bne @L_801F80A0
    li r0, 0x0
    b @L_801F80A4
    @L_801F80A0:
    li r0, 0x1
    @L_801F80A4:
    clrlwi. r0, r0, 24
    bne @L_801F80B4
    li r3, 0x0
    b @L_801F80EC
    @L_801F80B4:
    mr r3, r31
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    clrlwi r4, r3, 16
    li r3, 0x0
    li r5, 0x4
    li r6, 0x0
    bl fn_801FB1C0
    clrlwi r3, r3, 16
    bl fn_801FBD58
    bl fn_801FBD28
    bl fn_800FA280
    @L_801F80EC:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
