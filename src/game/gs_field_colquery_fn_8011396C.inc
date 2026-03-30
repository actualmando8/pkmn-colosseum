    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    cmpwi r3, 0x100
    bge @L_80113994
    cmpwi r3, 0xfd
    bge @L_8011398C
    b @L_80113994
@L_8011398C:
    li r3, 0x0
    b @L_801139AC
@L_80113994:
    bl fn_80115C48
    cmplwi r3, 0x0
    bne @L_801139A8
    li r3, 0x0
    b @L_801139AC
@L_801139A8:
    bl fn_80115840
@L_801139AC:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
