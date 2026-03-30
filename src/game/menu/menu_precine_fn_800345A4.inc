    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r4
    li r3, 0x0
    li r4, 0xe
    li r5, 0x0
    bl fn_8012A5B0
    lha r0, 0x6(r31)
    mr r6, r3
    cmpwi r0, 0x7f1
    beq @L_80034624
    bge @L_80034600
    cmpwi r0, 0x7ee
    beq @L_800346C0
    bge @L_800345F4
    cmpwi r0, 0x7bc
    beq @L_80034624
    b @L_800346F4
@L_800345F4:
    cmpwi r0, 0x7f0
    bge @L_80034650
    b @L_80034688
@L_80034600:
    cmpwi r0, 0x804
    beq @L_80034688
    bge @L_80034618
    cmpwi r0, 0x803
    bge @L_800346C0
    b @L_800346F4
@L_80034618:
    cmpwi r0, 0x806
    bge @L_800346F4
    b @L_80034650
@L_80034624:
    lis r4, lbl_802E61D8@ha
    mr r3, r31
    addi r4, r4, lbl_802E61D8@l
    lwz r4, 0xc(r4)
    subf r0, r4, r6
    orc r4, r6, r4
    rlwinm r0, r0, 31, 1, 31
    subf r0, r0, r4
    rlwinm r4, r0, 1, 31, 31
    bl fn_80109220
    b @L_800346F4
@L_80034650:
    lis r4, lbl_802E61D8@ha
    mr r3, r31
    addi r4, r4, lbl_802E61D8@l
    li r5, 0x0
    lwz r0, 0x8(r4)
    cmplw r6, r0
    blt @L_8003467C
    lwz r0, 0xc(r4)
    cmplw r6, r0
    bge @L_8003467C
    li r5, 0x1
@L_8003467C:
    rlwinm r4, r5, 0, 24, 31
    bl fn_80109220
    b @L_800346F4
@L_80034688:
    lis r4, lbl_802E61D8@ha
    mr r3, r31
    addi r4, r4, lbl_802E61D8@l
    li r5, 0x0
    lwz r0, 0x4(r4)
    cmplw r6, r0
    blt @L_800346B4
    lwz r0, 0x8(r4)
    cmplw r6, r0
    bge @L_800346B4
    li r5, 0x1
@L_800346B4:
    rlwinm r4, r5, 0, 24, 31
    bl fn_80109220
    b @L_800346F4
@L_800346C0:
    lis r4, lbl_802E61D8@ha
    mr r3, r31
    addi r4, r4, lbl_802E61D8@l
    li r5, 0x0
    lwz r0, 0x0(r4)
    cmplw r6, r0
    blt @L_800346EC
    lwz r0, 0x4(r4)
    cmplw r6, r0
    bge @L_800346EC
    li r5, 0x1
@L_800346EC:
    rlwinm r4, r5, 0, 24, 31
    bl fn_80109220
@L_800346F4:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
