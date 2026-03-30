    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    mr r30, r4
    stw r29, 0x14(r1)
    mr. r29, r3
    blt @L_8010FFF0
    cmpwi r29, 0x30
    blt @L_8010FFF8
@L_8010FFF0:
    li r3, 0x4
    b @L_80110034
@L_8010FFF8:
    bl fn_8010CBD0
    cmplwi r3, 0x0
    bne @L_8011000C
    li r3, 0x1
    b @L_80110034
@L_8011000C:
    mulli r4, r29, 0x14
    addi r4, r4, 0xa00
    add r4, r3, r4
    lhz r0, 0x10(r4)
    clrlwi. r0, r0, 31
    bne @L_8011002C
    li r3, 0x4
    b @L_80110034
@L_8011002C:
    mr r31, r4
    li r3, 0x0
@L_80110034:
    cmpwi r3, 0x0
    beq @L_80110040
    b @L_80110068
@L_80110040:
    cmpwi r30, 0x0
    beq @L_80110058
    lhz r0, 0x10(r31)
    rlwinm r0, r0, 0, 31, 29
    sth r0, 0x10(r31)
    b @L_80110064
@L_80110058:
    lhz r0, 0x10(r31)
    ori r0, r0, 0x2
    sth r0, 0x10(r31)
@L_80110064:
    li r3, 0x0
@L_80110068:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
