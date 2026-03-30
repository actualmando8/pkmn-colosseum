    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    lis r3, lbl_8036D3F0@ha
    lis r4, lbl_8036CC00@ha
    stw r0, 0x24(r1)
    lis r5, lbl_80275638@ha
    lis r6, lbl_80275650@ha
    addi r3, r3, lbl_8036D3F0@l
    stw r31, 0x1c(r1)
    addi r4, r4, lbl_8036CC00@l
    addi r5, r5, lbl_80275638@l
    addi r6, r6, lbl_80275650@l
    stw r30, 0x18(r1)
    li r7, 0x4c
    li r8, 0xac
    stw r29, 0x14(r1)
    bl fn_80193B30
    lis r9, fn_801BBC14@ha
    lis r8, fn_801BBBB4@ha
    lis r7, fn_801BE598@ha
    lis r3, fn_801BBCE0@ha
    addi r31, r8, fn_801BBBB4@l
    lis r29, lbl_8036D3F0@ha
    lis r30, lbl_8036D3F0@ha
    lis r12, lbl_8036D3F0@ha
    lis r10, lbl_8036D3F0@ha
    lis r8, lbl_8036D3F0@ha
    addi r12, r12, lbl_8036D3F0@l
    lis r6, fn_801BC8BC@ha
    addi r0, r9, fn_801BBC14@l
    addi r30, r30, lbl_8036D3F0@l
    addi r9, r6, fn_801BC8BC@l
    lis r6, lbl_8036D3F0@ha
    addi r8, r8, lbl_8036D3F0@l
    lis r5, fn_801BE2B4@ha
    addi r11, r7, fn_801BE598@l
    addi r10, r10, lbl_8036D3F0@l
    lis r4, fn_801BE85C@ha
    addi r7, r5, fn_801BE2B4@l
    addi r5, r4, fn_801BE85C@l
    addi r6, r6, lbl_8036D3F0@l
    lis r4, lbl_8036D3F0@ha
    addi r3, r3, fn_801BBCE0@l
    addi r29, r29, lbl_8036D3F0@l
    stw r0, 0x30(r30)
    addi r4, r4, lbl_8036D3F0@l
    stw r3, 0x2c(r29)
    stw r31, 0x38(r12)
    stw r11, 0x40(r10)
    stw r9, 0x44(r8)
    stw r7, 0x3c(r6)
    stw r5, 0x48(r4)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r0, 0x24(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
