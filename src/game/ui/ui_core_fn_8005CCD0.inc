    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    li r3, 0x1
    bl fn_801C40F0
    bl fn_8006B8E8
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_8005CD18
    li r31, 0x0
    li r3, 0x0
    li r4, 0xe
    bl fn_80129280
    stb r31, 0x1c(r3)
    li r3, 0x8ae
    li r4, 0x0
    bl fn_8019075C
@L_8005CD18:
    li r3, 0xd3
    bl fn_80102510
    bl fn_8025CD64
    lwz r3, lbl_8047A5A0(r13)
    lwz r3, 0x4314(r3)
    bl fn_801CB9D8
    lwz r3, lbl_8047A5A0(r13)
    bl fn_800E202C
    mr r31, r3
    clrlwi r0, r31, 16
    cmplwi r0, 0x0
    bne @L_8005CD5C
    lis r3, lbl_802678D8@ha
    li r4, 0x252
    addi r3, r3, lbl_802678D8@l
    la r5, lbl_8047BF28(r2)
    bl fn_80196E10
@L_8005CD5C:
    mr r3, r31
    bl fn_800E24B0
    mr r3, r31
    bl fn_800E209C
    li r0, 0x0
    stw r0, lbl_8047A5A0(r13)
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
