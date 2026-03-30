    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    lis r3, lbl_802705C0@ha
    lis r4, lbl_8036CB30@ha
    stw r0, 0x14(r1)
    lis r6, lbl_80315490@ha
    addi r5, r3, lbl_802705C0@l
    li r7, 0x54
    addi r3, r6, lbl_80315490@l
    addi r4, r4, lbl_8036CB30@l
    li r8, 0x24
    mr r6, r5
    bl fn_80193B30
    lis r5, fn_800DFE98@ha
    lis r4, lbl_80315490@ha
    lis r3, fn_800DF930@ha
    addi r5, r5, fn_800DFE98@l
    addi r4, r4, lbl_80315490@l
    addi r0, r3, fn_800DF930@l
    stw r5, 0x40(r4)
    stw r0, 0x44(r4)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
