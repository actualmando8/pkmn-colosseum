    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    cmpwi r3, 0x64
    lis r4, lbl_80272088@ha
    addi r31, r4, lbl_80272088@l
    li r30, 0x0
    beq @L_80113DA8
    bge @L_80113D9C
    cmpwi r3, 0x4
    beq @L_80113DB4
    bge @L_80113DC0
    cmpwi r3, 0x3
    bge @L_80113DA8
    b @L_80113DC0
@L_80113D9C:
    cmpwi r3, 0x66
    bge @L_80113DC0
    b @L_80113DB4
@L_80113DA8:
    lis r3, 0xf7
    addi r3, r3, 0x1000
    b @L_80113DD0
@L_80113DB4:
    lis r3, 0xf3
    addi r3, r3, 0x1000
    b @L_80113DD0
@L_80113DC0:
    rlwinm r0, r3, 23, 26, 31
    cmplwi r0, 0x2
    bne @L_80113DD0
    li r30, 0x1
@L_80113DD0:
    cmplwi r3, 0x0
    bne @L_80113DF4
    lis r4, lbl_8035B868@ha
    addi r3, r31, 0xdc
    addi r4, r4, lbl_8035B868@l
    crxor 6, 6, 6
    bl fn_800DD970
    li r3, 0x0
    b @L_80113F30
@L_80113DF4:
    bl fn_800F92D4
    rlwinm r0, r30, 0, 24, 31
    cmplwi r0, 0x0
    mr r0, r3
    beq @L_80113E84
    cmplwi r0, 0x0
    bne @L_80113E2C
    lis r4, lbl_8035B868@ha
    addi r3, r31, 0xf8
    addi r4, r4, lbl_8035B868@l
    crxor 6, 6, 6
    bl fn_800DD970
    li r3, 0x0
    b @L_80113F30
@L_80113E2C:
    addi r4, r31, 0x28
    bl fn_80191ECC
    cmplwi r3, 0x0
    bne @L_80113E58
    lis r4, lbl_8035B868@ha
    addi r3, r31, 0x118
    addi r4, r4, lbl_8035B868@l
    crxor 6, 6, 6
    bl fn_800DD970
    li r3, 0x0
    b @L_80113F30
@L_80113E58:
    lwz r3, 0x0(r3)
    lwz r0, 0x0(r3)
    cmplwi r0, 0x0
    bne @L_80113E84
    lis r4, lbl_8035B868@ha
    addi r3, r31, 0x140
    addi r4, r4, lbl_8035B868@l
    crxor 6, 6, 6
    bl fn_800DD970
    li r3, 0x0
    b @L_80113F30
@L_80113E84:
    mr r3, r0
    bl fn_800E4D18
    mr r30, r3
    cmplwi r30, 0x0
    bne @L_80113EBC
    lis r4, lbl_8035B868@ha
    addi r3, r31, 0x64
    addi r4, r4, lbl_8035B868@l
    crxor 6, 6, 6
    bl fn_800DD970
    addi r3, r31, 0x15c
    crxor 6, 6, 6
    bl fn_800DD970
    b @L_80113F2C
@L_80113EBC:
    li r4, 0x1
    bl fn_800E4014
    mr r3, r30
    bl fn_800EC1BC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_80113EF8
    mr r3, r30
    li r4, 0x0
    bl fn_800ECCA8
    lfs f1, -26376(r2)
    mr r3, r30
    bl fn_800EC9DC
    mr r3, r30
    bl fn_800EC990
@L_80113EF8:
    mr r3, r30
    bl fn_800EC1B0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_80113F2C
    mr r3, r30
    li r4, 0x0
    bl fn_800EC35C
    lfs f1, -26376(r2)
    mr r3, r30
    bl fn_800EC308
    mr r3, r30
    bl fn_800EC1E4
@L_80113F2C:
    mr r3, r30
@L_80113F30:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

