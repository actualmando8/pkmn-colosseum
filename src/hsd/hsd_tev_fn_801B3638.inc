    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    lwz r3, 0x8(r3)
    lwz r4, 0xc(r31)
    lwz r5, 0x10(r31)
    lwz r6, 0x14(r31)
    bl fn_800BC6F0
    lwz r0, 0x4(r31)
    cmplwi r0, 0x0
    bne @L_801B368C
    lwz r3, 0x8(r31)
    lwz r4, 0x18(r31)
    bl fn_800BC114
    lwz r3, 0x8(r31)
    li r4, 0x0
    li r5, 0x0
    bl fn_800BC52C
    b @L_801B375C
@L_801B368C:
    lwz r3, 0x8(r31)
    lwz r4, 0x18(r31)
    lwz r5, 0x30(r31)
    lwz r6, 0x2c(r31)
    lbz r7, 0x34(r31)
    lwz r8, 0x38(r31)
    bl fn_800BC228
    lwz r3, 0x8(r31)
    lwz r4, 0x1c(r31)
    lwz r5, 0x20(r31)
    lwz r6, 0x24(r31)
    lwz r7, 0x28(r31)
    bl fn_800BC1A0
    lwz r3, 0x8(r31)
    lwz r4, 0x3c(r31)
    lwz r5, 0x54(r31)
    lwz r6, 0x50(r31)
    lbz r7, 0x58(r31)
    lwz r8, 0x5c(r31)
    bl fn_800BC290
    lwz r3, 0x8(r31)
    lwz r4, 0x40(r31)
    lwz r5, 0x44(r31)
    lwz r6, 0x48(r31)
    lwz r7, 0x4c(r31)
    bl fn_800BC1E4
    lwz r3, 0x64(r31)
    lwz r4, 0x74(r31)
    lwz r5, 0x78(r31)
    lwz r6, 0x7c(r31)
    lwz r7, 0x80(r31)
    bl fn_800BC580
    lwz r3, 0x68(r31)
    lwz r0, 0x64(r31)
    cmpw r3, r0
    beq @L_801B3734
    lwz r3, 0x68(r31)
    lwz r4, 0x74(r31)
    lwz r5, 0x78(r31)
    lwz r6, 0x7c(r31)
    lwz r7, 0x80(r31)
    bl fn_800BC580
@L_801B3734:
    lwz r3, 0x8(r31)
    lwz r4, 0x64(r31)
    lwz r5, 0x68(r31)
    bl fn_800BC52C
    lwz r3, 0x8(r31)
    lwz r4, 0x6c(r31)
    bl fn_800BC454
    lwz r3, 0x8(r31)
    lwz r4, 0x70(r31)
    bl fn_800BC4C0
@L_801B375C:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
